
#ifndef __SWATCH_CORE_THREADPOOL_HXX__
#define __SWATCH_CORE_THREADPOOL_HXX__

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace swatch {
namespace core {

template<class OBJECT, class ResourceGuardType>
void ThreadPool::addTask(OBJECT* cmd,
    boost::function<void(OBJECT*, boost::shared_ptr<ResourceGuardType>, const XParameterSet&)> function,
    const boost::shared_ptr<ResourceGuardType>& resourceGuard,
    const XParameterSet& param) {
  // create packed_task
  boost::packaged_task<void> task(boost::bind(function, cmd, resourceGuard, boost::ref(param)));
  {
    // lock mutex
    boost::lock_guard<boost::mutex> guard(mQueueMutex);
    if (mStop) {
      throw OperationOnStoppedThreadPool(
          "ThreadPool is stopped, cannot schedule tasks.");
    }
    mTasks.push_back(boost::move(task));
  }
  mCondition.notify_one();
}

template<class OBJECT, class ResourceGuardType>
void ThreadPool::addTask( OBJECT* cmd , boost::function<void(OBJECT*, boost::shared_ptr<ResourceGuardType>)> function, const boost::shared_ptr<ResourceGuardType>& resourceGuard ) {
  // create packed_task
  boost::packaged_task<void> task(boost::bind(function, cmd, resourceGuard));
  {
    // lock mutex
    boost::lock_guard<boost::mutex> guard(mQueueMutex);
    if (mStop) {
      throw OperationOnStoppedThreadPool(
          "ThreadPool is stopped, cannot schedule tasks.");
    }
    mTasks.push_back(boost::move(task));
  }
  mCondition.notify_one();
}


} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_THREADPOOL_HXX__ */
