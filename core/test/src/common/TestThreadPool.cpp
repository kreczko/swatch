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
#include "swatch/core/test/DummyHandler.hpp"

using namespace swatch::logger;
using namespace swatch::core;

namespace swatch {
namespace core {
namespace test {

struct ThreadPoolSetup {
public:
  ThreadPoolSetup() :
          handler(),
          cmd1(),
          cmd2(),
          cmd3(),
          params(),
          wait_time_in_ms(10) {
    handler.Register<DummyCommand>("cmd1");
    handler.Register<DummyCommand>("cmd2");
    handler.Register<DummyCommand>("cmd3");

    cmd1 = handler.getCommand("cmd1");
    cmd2 = handler.getCommand("cmd2");
    cmd3 = handler.getCommand("cmd3");

    ((DummyCommand*) cmd1)->registerParameter("todo", xdata::String("thread"));
    ((DummyCommand*) cmd2)->registerParameter("todo", xdata::String("thread"));
    ((DummyCommand*) cmd3)->registerParameter("todo", xdata::String("thread"));

    ((DummyCommand*) cmd1)->registerParameter("milliseconds",
        xdata::Integer(wait_time_in_ms));
    ((DummyCommand*) cmd2)->registerParameter("milliseconds",
        xdata::Integer(wait_time_in_ms));
    ((DummyCommand*) cmd3)->registerParameter("milliseconds",
        xdata::Integer(wait_time_in_ms));

//     cmd1->setUseThreadPool(true);
//     cmd2->setUseThreadPool(true);
//     cmd3->setUseThreadPool(true);

  }
  ~ThreadPoolSetup() {

  }

  DummyHandler handler;
  Command *cmd1, *cmd2, *cmd3;
  XParameterSet params;
  unsigned int wait_time_in_ms;
};

BOOST_AUTO_TEST_SUITE( ThreadPoolTestSuite )
BOOST_FIXTURE_TEST_CASE(TolerantPool1, ThreadPoolSetup) {
  LOG(kInfo) << "Running ThreadPoolTestSuite/TolerantPool1";

  // create thread pool with 2 threads
  // does not finish queue
  // does not forcefully terminate running commands
  ThreadPool::getInstance(2, false, false);
  cmd1->exec(params);
  cmd2->exec(params);
  cmd3->exec(params);
  LOG(kInfo) << "cmd1 " << cmd1->getStatus().getProgress();
  // now wait for a bit
  // in 5ms cmd1 and cmd2 should start and due to nature of the pool finish
  // but cmd3 should not even start
  boost::this_thread::sleep_for(boost::chrono::milliseconds(wait_time_in_ms/2));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1->getState(), Command::kDone);
  BOOST_CHECK_EQUAL(cmd2->getState(), Command::kDone);
  // cmd3 should be in scheduled state
  BOOST_CHECK_EQUAL(cmd3->getState(), Command::kScheduled);
  BOOST_CHECK_EQUAL(cmd3->getStatus().getProgress(), 0);
}

BOOST_FIXTURE_TEST_CASE(TolerantPool2, ThreadPoolSetup) {
  LOG(kInfo) << "Running ThreadPoolTestSuite/TolerantPool2";

  // create thread pool with 2 threads
  // does not finish queue
  // does not forcefully terminate running commands
  ThreadPool::getInstance(2, false, false);
  cmd1->exec(params);
  cmd2->exec(params);
  cmd3->exec(params);
  LOG(kInfo) << "cmd1 " << cmd1->getStatus().getProgress();
  // now wait for a bit
  // in 18ms cmd1 and cmd2 should finish
  // and cmd3 start
  boost::this_thread::sleep_for(
      boost::chrono::milliseconds(int(wait_time_in_ms * 1.8)));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1->getState(), Command::kDone);
  BOOST_CHECK_EQUAL(cmd2->getState(), Command::kDone);
  // cmd3 should be done as well (since it started)
  BOOST_CHECK_EQUAL(cmd3->getState(), Command::kDone);
}

BOOST_FIXTURE_TEST_CASE(GoodGuyPool, ThreadPoolSetup) {
  LOG(kInfo) << "Running ThreadPoolTestSuite/GoodGuyPool";

  // create thread pool with 2 threads
  // this pool will wait until the queue is complete
  ThreadPool::getInstance(2, true, false);
  cmd1->exec(params);
  cmd2->exec(params);
  cmd3->exec(params);
  // now wait for a bit
  // in 5ms cmd1 and cmd2 should start but not finish
  // and cmd3 should not start
  // but our pool is nice and should wait until the queue is empty
  boost::this_thread::sleep_for(boost::chrono::milliseconds(wait_time_in_ms/2));
  // delete pool, forces all running commands to finish
  ThreadPool::reset();

  BOOST_CHECK_EQUAL(cmd1->getState(), Command::kDone);
  BOOST_CHECK_EQUAL(cmd2->getState(), Command::kDone);
  // cmd3 should be done as well for this nice pool
  BOOST_CHECK_EQUAL(cmd3->getState(), Command::kDone);
  // with more than 1% progress
  BOOST_CHECK_GT(cmd3->getStatus().getProgress(), 0.99);
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
