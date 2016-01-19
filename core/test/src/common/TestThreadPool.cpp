/*
 * TestThreadPool.cpp
 *
 *  Created on: 28 May 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"

#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"

using namespace swatch::logger;
using namespace swatch::core;

namespace swatch {
namespace core {
namespace test {

struct ThreadPoolSetup {
public:
  ThreadPoolSetup() :
      handler1("obj1"),
      handler2("obj2"),
      handler3("obj3"),
      cmd1( handler1.registerCommand<DummyCommand>("cmd") ),
      cmd2( handler2.registerCommand<DummyCommand>("cmd") ),
      cmd3( handler3.registerCommand<DummyCommand>("cmd") ),
      params(),
      wait_time_in_ms(10)
  {
    cmd1.registerParameter("todo", xdata::String("sleep"));
    cmd2.registerParameter("todo", xdata::String("sleep"));
    cmd3.registerParameter("todo", xdata::String("sleep"));

    cmd1.registerParameter("milliseconds", xdata::Integer(wait_time_in_ms));
    cmd2.registerParameter("milliseconds", xdata::Integer(wait_time_in_ms));
    cmd3.registerParameter("milliseconds", xdata::Integer(wait_time_in_ms));

  }

  ~ThreadPoolSetup() {}

  DummyActionableObject handler1, handler2, handler3;
  Command& cmd1;
  Command& cmd2;
  Command& cmd3;
  ReadOnlyXParameterSet params;
  unsigned int wait_time_in_ms;
};

BOOST_AUTO_TEST_SUITE( ThreadPoolTestSuite )
BOOST_FIXTURE_TEST_CASE(TolerantPool1, ThreadPoolSetup)
{
  // create thread pool with 2 threads
  // does not finish queue
  // does not forcefully terminate running commands
  ThreadPool::getInstance(2, false, false);
  cmd1.exec(params);
  cmd2.exec(params);
  cmd3.exec(params);
  LOG(kInfo) << "cmd1 " << cmd1.getStatus().getProgress();
  // now wait for a bit
  // in 5ms cmd1 and cmd2 should start and due to nature of the pool finish
  // but cmd3 should not even start
  boost::this_thread::sleep_for(boost::chrono::milliseconds(wait_time_in_ms/2));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1.getState(), ActionSnapshot::kDone);
  BOOST_CHECK_EQUAL(cmd2.getState(), ActionSnapshot::kDone);
  // cmd3 should be in scheduled state
  BOOST_CHECK_EQUAL(cmd3.getState(), ActionSnapshot::kScheduled);
  BOOST_CHECK_EQUAL(cmd3.getStatus().getProgress(), 0);
}

BOOST_FIXTURE_TEST_CASE(TolerantPool2, ThreadPoolSetup)
{
  // create thread pool with 2 threads
  // does not finish queue
  // does not forcefully terminate running commands
  ThreadPool::getInstance(2, false, false);
  cmd1.exec(params);
  cmd2.exec(params);
  cmd3.exec(params);
  BOOST_TEST_MESSAGE("cmd1 " << cmd1.getStatus().getProgress());
  // now wait for a bit
  // in 18ms cmd1 and cmd2 should finish
  // and cmd3 start
  boost::this_thread::sleep_for(
      boost::chrono::milliseconds(int(wait_time_in_ms * 1.8)));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1.getState(), ActionSnapshot::kDone);
  BOOST_CHECK_EQUAL(cmd2.getState(), ActionSnapshot::kDone);
  // cmd3 should be done as well (since it started)
  BOOST_CHECK_EQUAL(cmd3.getState(), ActionSnapshot::kDone);
}

BOOST_FIXTURE_TEST_CASE(GoodGuyPool, ThreadPoolSetup)
{
  // create thread pool with 2 threads
  // this pool will wait until the queue is complete
  ThreadPool::getInstance(2, true, false);
  cmd1.exec(params);
  cmd2.exec(params);
  cmd3.exec(params);
  // now wait for a bit
  // in 5ms cmd1 and cmd2 should start but not finish
  // and cmd3 should not start
  // but our pool is nice and should wait until the queue is empty
  boost::this_thread::sleep_for(boost::chrono::milliseconds(wait_time_in_ms/2));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1.getState(), ActionSnapshot::kDone);
  BOOST_CHECK_EQUAL(cmd2.getState(), ActionSnapshot::kDone);
  // cmd3 should be done as well for this nice pool
  BOOST_CHECK_EQUAL(cmd3.getState(), ActionSnapshot::kDone);
  // with more than 1% progress
  BOOST_CHECK_GT(cmd3.getStatus().getProgress(), 0.99);
}

/*
 * This is causing
 * terminate called after throwing an instance of 'boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::lock_error> >'
terminate called recursively
  what():  boost: mutex lock failed in pthread_mutex_lock: Invalid argument
Aborted
 *
 */
//BOOST_FIXTURE_TEST_CASE(AngryPool, ThreadPoolSetup) {
//  LOG(kInfo) << "Running ThreadPoolTestSuite/AngryPool";
//
//  // create thread pool with 2 threads
//  // does not finish queue
//  // does forcefully terminate running commands
//  ThreadPool::getInstance(2, false, true);
//  cmd1->exec(params);
//  cmd2->exec(params);
//  cmd3->exec(params);
//  LOG(kInfo) << "cmd1 " << cmd1->getProgress();
//  // now wait for a bit
//  // in 5ms cmd1 and cmd2 should start
//  // but not cmd3
//  boost::this_thread::sleep_for(boost::chrono::milliseconds(wait_time_in_ms/2));
//  // delete pool, forces all running commands (cmd1 & cmd2) to cancel
//  try {
//    ThreadPool::reset();
//  } catch (...) {
//
//  }
//
//  BOOST_CHECK_EQUAL(cmd1->getStatus(), Command::kRunning);
//  BOOST_CHECK_EQUAL(cmd2->getStatus(), Command::kRunning);
//  // cmd3 should be in running state
//  BOOST_CHECK_EQUAL(cmd3->getStatus(), Command::kInitial);
//}

BOOST_AUTO_TEST_SUITE_END() // ThreadPoolTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
