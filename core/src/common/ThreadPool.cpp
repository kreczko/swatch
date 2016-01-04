#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"

using namespace swatch::logger;

namespace swatch {
namespace core {


boost::mutex ThreadPool::sInstanceMutex;
ThreadPool* ThreadPool::sInstance = NULL;


ThreadPool& ThreadPool::getInstance(size_t n_threads,
    bool run_until_queue_empty, bool force_thread_cancellation) {
  boost::unique_lock<boost::mutex> lock(ThreadPool::sInstanceMutex);
  if (!sInstance) {
    sInstance = new ThreadPool(n_threads, run_until_queue_empty,
        force_thread_cancellation);
  }
  return *ThreadPool::sInstance;
}


void ThreadPool::reset() {
  boost::unique_lock<boost::mutex> lock(ThreadPool::sInstanceMutex);
  delete ThreadPool::sInstance;
  ThreadPool::sInstance = NULL;
}


ThreadPool::ThreadPool(size_t n_threads, bool run_until_queue_empty,
    bool force_thread_cancellation) :
        mNumThreads(n_threads),
        mWorkers(),
        mTasks(),
        mQueueMutex(),
        mCondition(),
        mStop(false),
        mForceThreadCancellation(force_thread_cancellation) {
  for (size_t i = 0; i < mNumThreads; ++i)
    mWorkers.create_thread(Worker(*this, run_until_queue_empty));
}


ThreadPool::~ThreadPool() {
  {
    boost::unique_lock<boost::mutex> lock(mQueueMutex);
    // set the pool to stop
    mStop = true;
  }
  // notify all waiters
  mCondition.notify_all();

  // join them
  if (mForceThreadCancellation) {
    // bad things happen if you interrupt a thread
    try {
      mWorkers.interrupt_all();
    } catch (...) {
    }
  } else
    mWorkers.join_all();
}


Worker::Worker(ThreadPool &pool, bool run_until_queue_empty) :
        mPool(pool),
        mRunUntilQueueEmpty(run_until_queue_empty) {

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
      boost::unique_lock<boost::mutex> lock(mPool.mQueueMutex);
      // look for work
      while (!mPool.mStop && mPool.mTasks.empty()) {
        //nothing to do, please wait
        mPool.mCondition.wait(lock);
      }
      if (mPool.mStop && (!mRunUntilQueueEmpty || mPool.mTasks.empty()))
        //pool stopped and so shall I
        return;

      //otherwise get a task from the queue
      task = boost::move(mPool.mTasks.front());
      mPool.mTasks.pop_front();
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
