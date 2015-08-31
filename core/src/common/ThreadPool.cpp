#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"

using namespace swatch::logger;

namespace swatch {
namespace core {


boost::mutex ThreadPool::mutex_;
ThreadPool* ThreadPool::instance_ = NULL;


ThreadPool& ThreadPool::getInstance(size_t n_threads,
    bool run_until_queue_empty, bool force_thread_cancellation) {
  boost::unique_lock<boost::mutex> lock(ThreadPool::mutex_);
  if (!instance_) {
    instance_ = new ThreadPool(n_threads, run_until_queue_empty,
        force_thread_cancellation);
  }
  return *ThreadPool::instance_;
}


void ThreadPool::reset() {
  boost::unique_lock<boost::mutex> lock(ThreadPool::mutex_);
  delete ThreadPool::instance_;
  ThreadPool::instance_ = NULL;
}


ThreadPool::ThreadPool(size_t n_threads, bool run_until_queue_empty,
    bool force_thread_cancellation) :
        n_threads_(n_threads),
        workers_(),
        tasks_(),
        queue_mutex_(),
        condition_(),
        stop_(false),
        force_thread_cancellation_(force_thread_cancellation) {
  for (size_t i = 0; i < n_threads_; ++i)
    workers_.create_thread(Worker(*this, run_until_queue_empty));
}


ThreadPool::~ThreadPool() {
  {
    boost::unique_lock<boost::mutex> lock(queue_mutex_);
    // set the pool to stop
    stop_ = true;
  }
  // notify all waiters
  condition_.notify_all();

  // join them
  if (force_thread_cancellation_) {
    // bad things happen if you interrupt a thread
    try {
      workers_.interrupt_all();
    } catch (...) {
    }
  } else
    workers_.join_all();
}


Worker::Worker(ThreadPool &pool, bool run_until_queue_empty) :
        pool_(pool),
        run_until_queue_empty_(run_until_queue_empty) {

}


void Worker::operator()() {
  while (true)
  {
    // N.B: Declare task object within the loop, so that it's destroyed after task has completed ...
    // ... otherwise if task object isn't destroyed after task has completed execution, then ...
    // ... the object instances that form arguments of the task (e.g. shared_ptr<ActionableObject::BusyGuard>) ...
    // ... will still exist until the worker is given its next 
    boost::packaged_task<void> task;
    
    {
      // get lock
      boost::unique_lock<boost::mutex> lock(pool_.queue_mutex_);
      // look for work
      while (!pool_.stop_ && pool_.tasks_.empty()) {
        //nothing to do, please wait
        pool_.condition_.wait(lock);
      }
      if (pool_.stop_ && (!run_until_queue_empty_ || pool_.tasks_.empty()))
        //pool stopped and so shall I
        return;

      //otherwise get a task from the queue
      task = boost::move(pool_.tasks_.front());
      pool_.tasks_.pop_front();
    } // release lock

    //execute the task
    try {
      task();
    } catch (std::exception& e) {
      LOG(swatch::logger::kError) << e.what();
    }
  }
}


} // namespace core
} // namespace swatch
