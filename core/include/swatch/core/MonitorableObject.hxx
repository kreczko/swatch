#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

template < typename T >
void MonitorableObject::registerMonitorable( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<Metric,T>::value) );
  if (mMonitorables.count(aId)) throw MonitorableAlreadyExistsInMonitorableObject(aId);
  mMonitorables.emplace( aId , new T(this) );
}


}
}
