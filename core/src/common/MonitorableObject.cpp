
#include "swatch/core/MonitorableObject.hpp"
#include <boost/foreach.hpp>

using namespace std;

namespace swatch {
namespace core {

MonitorableObject::MonitorableObject( const std::string& aId ) :
    Object( aId )
{
}

MonitorableObject::MonitorableObject( const std::string& aId , const XParameterSet& aParams ) :
    Object( aId , aParams )
{
}

MonitorableObject::~MonitorableObject()
{
  BOOST_FOREACH( tMonitorableMap::value_type p, mMonitorables) {
    delete p.second;
  }
  mMonitorables.clear();
  
}

Metric* MonitorableObject::getMonitorable( const std::string& aId )
{
  try {
    return mMonitorables.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw MonitorableNotFoundInMonitorableObject(aId);
  }
  return NULL;
}

std::set<std::string> MonitorableObject::getMonitorables() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( tMonitorableMap::value_type p, mMonitorables) {
    lNames.insert( p.first );
  }
  return lNames;
}

void MonitorableObject::registerMonitorable( const std::string& aId , Metric* aMetric )
{
  if (mMonitorables.count(aId)) throw MonitorableAlreadyExistsInMonitorableObject(aId);
  mMonitorables.insert( std::make_pair( aId , aMetric ) );
}

}
}
