#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace swatch {
namespace core {
template<class OBJECT>
void ThreadPool::addTask(OBJECT* cmd,
    boost::function<void(OBJECT*, XParameterSet&)> function,
    XParameterSet& param) {
  // create packed_task
  boost::packaged_task<void> task(boost::bind(function, cmd, param));
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

