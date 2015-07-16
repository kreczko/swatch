
#ifndef __SWATCH_CORE_THREADPOOL_HXX__
#define __SWATCH_CORE_THREADPOOL_HXX__

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace swatch {
namespace core {

template<class OBJECT>
void ThreadPool::addTask(OBJECT* cmd,
    boost::function<void(OBJECT*, const XParameterSet&)> function,
    const XParameterSet& param) {
  // create packed_task
  boost::packaged_task<void> task(boost::bind(function, cmd, boost::ref(param)));
  {
    // lock mutex
    boost::lock_guard<boost::mutex> guard(queue_mutex_);
    if (stop_) {
      throw OperationOnStoppedThreadPool(
          "ThreadPool is stopped, cannot schedule tasks.");
    }
    tasks_.push_back(boost::move(task));
  }
  condition_.notify_one();
}

template<class OBJECT>
void ThreadPool::addTask( OBJECT* cmd , boost::function<void(OBJECT*)> function ) {
  // create packed_task
  boost::packaged_task<void> task(boost::bind(function, cmd));
  {
    // lock mutex
    boost::lock_guard<boost::mutex> guard(queue_mutex_);
    if (stop_) {
      throw OperationOnStoppedThreadPool(
          "ThreadPool is stopped, cannot schedule tasks.");
    }
    tasks_.push_back(boost::move(task));
  }
  condition_.notify_one();
}


} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_THREADPOOL_HXX__ */
