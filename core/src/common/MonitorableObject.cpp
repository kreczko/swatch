
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


std::vector<std::string> MonitorableObject::getMetrics() const
{
  std::vector<std::string> lNames;
  BOOST_FOREACH( tMonitorableMap::value_type p, mMonitorables) {
    lNames.push_back( p.first );
  }
  return lNames;
}


AbstractMetric& MonitorableObject::getMetric( const std::string& aId )
{
  try {
    return *mMonitorables.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw MonitorableNotFoundInMonitorableObject("MonitorableObject \"" + this->path() + "\" does not contain metric of ID \"" + aId + "\"");
  }
}


StatusFlag MonitorableObject::getStatus() const
{
  StatusFlag result = kGood;
  
  std::vector<std::string> childIds = getChildren();

  for(std::vector<std::string>::const_iterator it=childIds.begin(); it != childIds.end(); it++)
  {
    if(swatch::core::MonitorableObject* monObj = dynamic_cast<swatch::core::MonitorableObject*>(this->getObj(*it)))
    {
      result = result & monObj->getStatus();
    }
  }
  
  BOOST_FOREACH( tMonitorableMap::value_type p, mMonitorables) {
    result = result & p.second->getValue().first;
  }
  
  return result;
}


}
}
