#include <boost/test/unit_test.hpp>


// Standard headers
#include <stdexcept>

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"


using namespace swatch::logger;

namespace swatch {
namespace core {
namespace test {


struct BusyMonitoringGuardTestSetup {
  BusyMonitoringGuardTestSetup();
  ~BusyMonitoringGuardTestSetup();

  boost::shared_ptr<DummyActionableObject> obj;
  Command& waitingCmd;
  Command& anotherCmd;
  DummyActionableObject::MonChild& child;
};


BusyMonitoringGuardTestSetup::BusyMonitoringGuardTestSetup() : 
  obj(new DummyActionableObject("dummyObj"), ActionableObject::Deleter()),
  waitingCmd(obj->registerCommand<DummyActionableObject::WaitCommand>("waitCommand")),
  anotherCmd(obj->registerCommand<DummyCommand>("anotherCommand")),
  child(obj->addMonitorable(new DummyActionableObject::MonChild("monitorableChild", *obj)))
{
}

BusyMonitoringGuardTestSetup::~BusyMonitoringGuardTestSetup()
{
}


class UpdateMetrics {
public:
  UpdateMetrics(MonitorableObject& aObj);
  ~UpdateMetrics();

  void operator()();

private:
  MonitorableObject& mObj;
};


UpdateMetrics::UpdateMetrics(MonitorableObject& aObj) :
  mObj(aObj)
{
}

UpdateMetrics::~UpdateMetrics() {}

void UpdateMetrics::operator()()
{
  MetricUpdateGuard lGuard(mObj);
  mObj.updateMetrics(lGuard);
}


class RunCommand {
public:
  RunCommand(Command& aCmd, const ReadOnlyXParameterSet& aParams);
  ~RunCommand();
  
  void operator()();

private:
  Command& mCmd;
  ReadOnlyXParameterSet mParams;
};


RunCommand::RunCommand(Command& aCmd, const ReadOnlyXParameterSet& aParams) : 
  mCmd(aCmd),
  mParams(aParams)
{
}

RunCommand::~RunCommand()
{
}

void RunCommand::operator ()()
{
  mCmd.exec(mParams, false);  
}



BOOST_AUTO_TEST_SUITE( BusyMonitoringGuardsTestSuite )


BOOST_FIXTURE_TEST_CASE(TestBusyThenMetricWriteGuard, BusyMonitoringGuardTestSetup)
{
  LOG(kInfo) << "Running BusyMonitoringGuardsTestSuite/TestBusyThenMetricWriteGuard";

  // 0) Check that metrics are "unknown" and command status "initialised" before proceed with tests.
  MetricSnapshot lMetricSnapshot = obj->getDummyMetric().getSnapshot();
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_sec, 0);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_usec, 0);
  BOOST_REQUIRE_EQUAL(waitingCmd.getState(), ActionSnapshot::kInitial);

  // 1a) Run command in another thread (via thread pool)
  obj->pleaseWaitNextTime();
  waitingCmd.exec(ReadOnlyXParameterSet());
  // 1b) Spawn another thread that starts updating metrics
  UpdateMetrics lUpdateMetricsFunctor(*obj);
  boost::thread lUpdateMetricsThread(lUpdateMetricsFunctor);

  // 2) Check that busy guard has updated the actionable object's status
  ActionableObject::Status_t lObjStatus = obj->getStatus();
  BOOST_REQUIRE_EQUAL(lObjStatus.isRunning(), true);
  BOOST_REQUIRE_NE(lObjStatus.getRunningActions().size(), size_t(0));
  BOOST_REQUIRE_EQUAL(lObjStatus.isUpdatingMetrics(), false);
  BOOST_REQUIRE_NE(waitingCmd.getState(), ActionSnapshot::kInitial);
  
  // 3) Sleep until command is waiting; then check that metric still hasn't been updated
  while ( obj->getWaitingThread() == boost::thread::id() )
  {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2));
  }
  boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
  BOOST_REQUIRE_EQUAL(waitingCmd.getState(), ActionSnapshot::kRunning);
  lMetricSnapshot = obj->getDummyMetric().getSnapshot();
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_sec, 0);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_usec, 0);

  // 4) Let threads continue from command into monitoring update; check that metric's value has been updated 
  obj->pleaseContinue();
  while ( obj->getWaitingThread() != lUpdateMetricsThread.get_id() )
  {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2));
  }
  BOOST_REQUIRE_EQUAL(waitingCmd.getState(), ActionSnapshot::kDone);
  lMetricSnapshot = obj->getDummyMetric().getSnapshot();
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getStatusFlag(), swatch::core::kNoLimit);
  BOOST_REQUIRE_NE(lMetricSnapshot.getUpdateTimestamp().tv_sec, 0);
  BOOST_REQUIRE_NE(lMetricSnapshot.getUpdateTimestamp().tv_usec, 0);
  lObjStatus = obj->getStatus();
  BOOST_REQUIRE_EQUAL(lObjStatus.isRunning(), false);
  BOOST_REQUIRE_EQUAL(lObjStatus.getRunningActions().size(), size_t(0));
  BOOST_REQUIRE_EQUAL(lObjStatus.isUpdatingMetrics(), true);
  
  // 5) Let threads continue out of monitoring update; check that object's status has been updated
  obj->pleaseContinue();
  lUpdateMetricsThread.join();
  BOOST_CHECK_EQUAL(lObjStatus.isRunning(), false);
  BOOST_CHECK_EQUAL(lObjStatus.getRunningActions().size(), size_t(0));
  BOOST_CHECK_EQUAL(lObjStatus.isUpdatingMetrics(), true);
}


BOOST_FIXTURE_TEST_CASE(TestMetricWriteThenBusyGuard, BusyMonitoringGuardTestSetup)
{
  LOG(kInfo) << "Running BusyMonitoringGuardsTestSuite/TestMetricWriteThenBusyGuard";

  // 0) Check that metrics are "unknown" and command status "initialised" before proceed with tests.
  MetricSnapshot lMetricSnapshot = obj->getDummyMetric().getSnapshot();
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_sec, 0);
  BOOST_REQUIRE_EQUAL(lMetricSnapshot.getUpdateTimestamp().tv_usec, 0);
  BOOST_REQUIRE_EQUAL(waitingCmd.getState(), ActionSnapshot::kInitial);
  
  // 1) Spawn a thread that will update metrics 
  obj->pleaseWaitNextTime();
  UpdateMetrics lUpdateMetricsFunctor(*obj);
  boost::thread lUpdateMetricsThread(lUpdateMetricsFunctor);
  
  // 2) Sleep until updateMetrics is waiting; then spawn a thread that runs a command
  while ( obj->getWaitingThread() != lUpdateMetricsThread.get_id() )
  {
    boost::this_thread::sleep_for( boost::chrono::milliseconds(2) );
  }
  lMetricSnapshot = obj->getDummyMetric().getSnapshot();
  BOOST_CHECK_NE(lMetricSnapshot.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMetricSnapshot.getUpdateTimestamp().tv_sec, 0);
  BOOST_CHECK_NE(lMetricSnapshot.getUpdateTimestamp().tv_usec, 0);
  BOOST_CHECK_EQUAL(waitingCmd.getState(), ActionSnapshot::kInitial);
  ActionableObject::Status_t lObjStatus = obj->getStatus();
  BOOST_CHECK( lObjStatus.isUpdatingMetrics() );
  BOOST_REQUIRE( ! lObjStatus.isRunning() );
  BOOST_REQUIRE( lObjStatus.getRunningActions().empty() );

  RunCommand lRunCommandFunctor(waitingCmd, ReadOnlyXParameterSet());
  boost::thread lRunCommandThread(lRunCommandFunctor);
  
  // 3) Sleep until other thread is in BusyGuard CTOR; then check that can't have two commands waiting at same time
  while ( ! obj->getStatus().isActionWaitingToRun()  )
  {
    boost::this_thread::sleep_for( boost::chrono::milliseconds(2) );
  }
  BOOST_CHECK_THROW( anotherCmd.exec(ReadOnlyXParameterSet()), std::runtime_error );

  // 4) Let metric update thread continue, and sleep until command is in dummy's "wait" method;
  //    ... then check that actionable object's status is correct
  obj->pleaseContinue();
  lUpdateMetricsThread.join();
  while ( obj->getWaitingThread() != lRunCommandThread.get_id() )
  {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2));
  }

  lObjStatus = obj->getStatus();
  BOOST_CHECK( ! lObjStatus.isUpdatingMetrics() );
  BOOST_REQUIRE( lObjStatus.isRunning() );
  BOOST_REQUIRE( ! lObjStatus.isActionWaitingToRun() );
  BOOST_REQUIRE_EQUAL( lObjStatus.getRunningActions().size(), size_t(1) );
  BOOST_REQUIRE_EQUAL( lObjStatus.getRunningActions().at(0), &waitingCmd );
  BOOST_REQUIRE_EQUAL( waitingCmd.getState(), ActionSnapshot::kRunning );

  // 5) Let "run command" thread continue; once it's finished, check that actionable object's status is correct
  obj->pleaseContinue();
  lRunCommandThread.join();
  lObjStatus = obj->getStatus();
  BOOST_CHECK( ! lObjStatus.isUpdatingMetrics() );
  BOOST_CHECK( ! lObjStatus.isRunning() );
  BOOST_CHECK( ! lObjStatus.isActionWaitingToRun() );
  BOOST_CHECK( lObjStatus.getRunningActions().empty() );

  // 5) Finally, check that the other command has never been run
  BOOST_CHECK_EQUAL( anotherCmd.getState(), ActionSnapshot::kInitial );
}


BOOST_AUTO_TEST_SUITE_END() // BusyMonitoringGuardsTestSuite


} // end namespace test
} // end namespace core
} // end namespace swatch