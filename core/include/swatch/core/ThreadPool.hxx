
#ifndef __SWATCH_CORE_THREADPOOL_HXX__
#define __SWATCH_CORE_THREADPOOL_HXX__

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace swatch {
namespace core {

template<class OBJECT, class ResourceGuardType>
void ThreadPool::addTask(OBJECT* aCmd,
    boost::function<void(OBJECT*, boost::shared_ptr<ResourceGuardType>, const XParameterSet&)> aFunction,
    const boost::shared_ptr<ResourceGuardType>& aResourceGuard,
    const XParameterSet& aParamSet)
{
  // create packed_task
  boost::packaged_task<void> task(boost::bind(aFunction, aCmd, aResourceGuard, boost::ref(aParamSet)));
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
void ThreadPool::addTask( OBJECT* aCmd , boost::function<void(OBJECT*, boost::shared_ptr<ResourceGuardType>)> aFunction, const boost::shared_ptr<ResourceGuardType>& aResourceGuard )
{
  // create packed_task
  boost::packaged_task<void> task(boost::bind(aFunction, aCmd, aResourceGuard));
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
