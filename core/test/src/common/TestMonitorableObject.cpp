/*
 * FSM_test.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// SWATCH headers
#include "swatch/core/MetricConditions.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyMonitorableObjects.hpp"


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {


BOOST_AUTO_TEST_SUITE( MonitorableObjectTestSuite )


BOOST_AUTO_TEST_CASE(MetricWriteGuardRequiresStatus)
{
  LOG(kInfo) << "Running MonitorableObjectTestSuite/MetricWriteGuardRequiresStatus";

  // 1) DummyMonitorableObject: Doesn't have a status member variable (inheriting from AbstractMonitorableObject);
  // ... hence: MetricWriteGuard CTOR should throw; updateMetrics should throw before calling retrieveMetricValues
  DummyMonitorableObject obj;
  // 1a) Require that assumptions are correct (to prevent false positives later)
  BOOST_REQUIRE_EQUAL(obj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(obj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(obj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  //TODO: same check for each metric
  // 1b) Try to create guard or update metrics
  BOOST_CHECK_THROW( MetricWriteGuard lMetricWriteGuard(obj), std::runtime_error);
  BOOST_CHECK_THROW( obj.updateMetrics(), std::runtime_error);
  // 1c) Check that retriveMetricValues() hasn't been called
  BOOST_CHECK_EQUAL(obj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(obj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(obj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);


  // 2) DummyMasterMonitorableObject does have a status member variable (inheriting from AbstractMonitorableObject);
  // ... hence, all should be good
  DummyMasterMonitorableObject masterObj;
  // 2a) Require that assumptions are correct (to prevent false positives later)
  BOOST_REQUIRE_EQUAL(masterObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(masterObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(masterObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  // 2b) Try to create guard or update metrics - should succeed without throwing
  BOOST_CHECK_NO_THROW( MetricWriteGuard lMetricWriteGuard2(masterObj) );
  BOOST_CHECK_NO_THROW( masterObj.updateMetrics() );
  // 2c) Check that retriveMetricValues() has been called
  BOOST_CHECK_NE(masterObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(masterObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(masterObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
}


BOOST_AUTO_TEST_CASE(IncorrectMetricWriteGuardCaseA)
{
  LOG(kInfo) << "Running MonitorableObjectTestSuite/IncorrectMetricWriteGuardCaseA";
  // UpdateMetrics should throw if monitorable object is given a MetricWriteGuard for a different object
  // Case A: Using write guard from one master with another master

  DummyMasterMonitorableObject lMasterObj1, lMasterObj2;

  MetricWriteGuard lWriteGuard(lMasterObj1);
  
  // 1) Guard should work fine with lMasterObj1
  BOOST_REQUIRE_EQUAL(lMasterObj1.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj1.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj1.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NO_THROW( lMasterObj1.updateMetrics(lWriteGuard) );
  BOOST_CHECK_NE(lMasterObj1.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj1.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj1.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj1.getDisabledMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);

  // 2) Using object 1's write guard with object 2: updateMetrics should throw before calling retrieveMetricValues
  BOOST_REQUIRE_EQUAL(lMasterObj2.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj2.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj2.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_THROW( lMasterObj2.updateMetrics(lWriteGuard), std::runtime_error );
  BOOST_CHECK_EQUAL(lMasterObj2.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(lMasterObj2.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(lMasterObj2.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
}


BOOST_AUTO_TEST_CASE(IncorrectMetricWriteGuardCaseB)
{
  // UpdateMetrics should throw if monitorable object is given a MetricWriteGuard for a different object
  // Case B: Using write guard with a DummyMonitorableObject whose status pointer hasn't been set yet

  DummyMasterMonitorableObject lMasterObj;
  MetricWriteGuard lWriteGuard(lMasterObj);
  
  // 1) Guard should work fine with lMasterObj1
  BOOST_REQUIRE_EQUAL(lMasterObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lMasterObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NO_THROW( lMasterObj.updateMetrics(lWriteGuard) );
  BOOST_CHECK_NE(lMasterObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_NE(lMasterObj.getDisabledMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);

  // 2) Using object 1's write guard with object 2: updateMetrics should throw before calling retrieveMetricValues
  DummyMonitorableObject lObj;
  BOOST_REQUIRE_EQUAL(lObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_REQUIRE_EQUAL(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_THROW( lObj.updateMetrics(lWriteGuard), std::runtime_error );
  BOOST_CHECK_EQUAL(lObj.getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  BOOST_CHECK_EQUAL(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
}


BOOST_AUTO_TEST_CASE(SettingStatusPointer_AddChildFirst)
{
  LOG(kInfo) << "Running MonitorableObjectTestSuite/SettingStatusPointer_AddChildFirst";
  // Check that the addMonitorable method sets the child and grandchild's status pointers correctly
  //  - Master
  //    - Child
  //      - Grandchild
  // Case A: Add child to master; then add grandchild to child
  
  DummyMasterMonitorableObject lMaster;
  MetricWriteGuard lWriteGuard(lMaster);
  DummyMonitorableObject& lChild = dynamic_cast<DummyMonitorableObject&>(lMaster.addMonitorable( new DummyMonitorableObject() ));
  DummyMonitorableObject& lGrandChild = dynamic_cast<DummyMonitorableObject&>(lChild.addMonitorable( new DummyMonitorableObject() ));

  const std::vector<DummyMonitorableObject*> lObjVec = {&lMaster, &lChild, &lGrandChild};
  std::vector<DummyMonitorableObject*>::const_iterator lObjIt;

  for(lObjIt=lObjVec.begin(); lObjIt!=lObjVec.end(); lObjIt++)
  {
    DummyMonitorableObject& lObj = **lObjIt;
    BOOST_TEST_MESSAGE("Object:" << lObj.getPath());
    BOOST_REQUIRE_EQUAL(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_REQUIRE_EQUAL(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_CHECK_NO_THROW( lObj.updateMetrics(lWriteGuard) );
    BOOST_CHECK_NE(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_CHECK_NE(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  }
}


BOOST_AUTO_TEST_CASE(SettingStatusPointer_AddGrandChildFirst)
{
  LOG(kInfo) << "Running MonitorableObjectTestSuite/SettingStatusPointer_AddGrandChildFirst";
  // Check that the addMonitorable method sets the child and grandchild's status pointers correctly
  //  - Master
  //    - Child
  //      - Grandchild
  // Case B: Add grandchild to child; then add child to master
  
  DummyMasterMonitorableObject lMaster;
  MetricWriteGuard lWriteGuard(lMaster);
  DummyMonitorableObject* lChild = new DummyMonitorableObject();
  DummyMonitorableObject& lGrandChild = dynamic_cast<DummyMonitorableObject&>(lChild->addMonitorable( new DummyMonitorableObject() ));
  lMaster.addMonitorable(lChild);
  
  const std::vector<DummyMonitorableObject*> lObjVec = {&lMaster, lChild, &lGrandChild};
  std::vector<DummyMonitorableObject*>::const_iterator lObjIt;

  for(lObjIt=lObjVec.begin(); lObjIt!=lObjVec.end(); lObjIt++)
  {
    DummyMonitorableObject& lObj = **lObjIt;
    BOOST_TEST_MESSAGE("Object:" << lObj.getPath());
    BOOST_REQUIRE_EQUAL(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_REQUIRE_EQUAL(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_CHECK_NO_THROW( lObj.updateMetrics(lWriteGuard) );
    BOOST_CHECK_NE(lObj.getNonCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
    BOOST_CHECK_NE(lObj.getCriticalMetric().getSnapshot().getStatusFlag(), swatch::core::kUnknown);
  }
}


BOOST_AUTO_TEST_CASE(GoodMonitorableObject) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/GoodMonitorableObject";
  DummyMasterMonitorableObject m;
  m.updateMetrics();
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getSnapshot();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms2.getValue(), "2");
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(ms2.getStatusFlag(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(m.getStatusFlag(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(CriticalFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/CriticalFailure";
  DummyMasterMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setCriticalMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getSnapshot();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "-232");
  BOOST_CHECK_EQUAL(ms2.getValue(), "2");
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kError);
  BOOST_CHECK_EQUAL(ms2.getStatusFlag(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(m.getStatusFlag(), swatch::core::StatusFlag::kError);
}

BOOST_AUTO_TEST_CASE(NonCriticalFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/NonCriticalFailure";
  DummyMasterMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setNonCriticalMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getSnapshot();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getSnapshot();
  BOOST_CHECK_EQUAL(ms2.getMonitoringStatus(), monitoring::Status::kNonCritical);
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms2.getValue(), "-232");
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kGood);
  // non-critical metric fails
  BOOST_CHECK_EQUAL(ms2.getStatusFlag(), swatch::core::StatusFlag::kError);
  // but the MonitoringObject status should be still good
  BOOST_CHECK_EQUAL(m.getStatusFlag(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(DisabledFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/DisabledFailure";
  DummyMasterMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setDisabledMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyDisabledInteger").getSnapshot();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), monitoring::Status::kDisabled);
  BOOST_CHECK_EQUAL(ms.getValue(), "-232");
  // nothing changes for disabled Metric
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kNoLimit);
  // and no effect on MonitoringObject either
  BOOST_CHECK_EQUAL(m.getStatusFlag(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(DisabledFailureNoUnknown) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/DisabledFailure";
  DummyMasterMonitorableObject m;
  m.updateMetrics();
  // at this point the time of last update should be 0
  // normally MonitoringObject will attempt to set the value
  // to unknown, but that should not be the case for disabled metrics
  MetricSnapshot ms = m.getMetric("DummyDisabledInteger").getSnapshot();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), monitoring::Status::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kNoLimit);
  BOOST_CHECK_EQUAL(m.getStatusFlag(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_SUITE_END() // MonitorableObjectTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
