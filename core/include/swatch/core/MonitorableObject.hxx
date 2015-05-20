#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

#include "Metric.hpp"

namespace swatch {
namespace core {

template <typename DataType, typename ObjectType> 
void MonitorableObject::registerMetric( const std::string& aId, ObjectType& aObj, typename Metric<DataType, ObjectType>::ParentMemberFunctionPtr aRetrieveValueFunc, DataType aMinGoodValue, DataType aMaxGoodValue)
{
  // BOOST_STATIC_ASSERT( (boost::is_base_of<Metric,T>::value) );
  assert(static_cast<void*>(&aObj) == static_cast<void*>(this) );
  
  if (mMonitorables.count(aId)) throw MonitorableAlreadyExistsInMonitorableObject(aId);
  mMonitorables.emplace( aId , new Metric<DataType, ObjectType>(aObj, aRetrieveValueFunc, aMinGoodValue, aMaxGoodValue));
}


}
}
