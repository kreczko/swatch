
#include "swatch/core/MonitorableObject.hpp"


#include "boost/foreach.hpp"


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
  BOOST_FOREACH( tMetricMap::value_type p, metrics_) {
    delete p.second;
  }
  metrics_.clear();
}


std::vector<std::string> MonitorableObject::getMetrics() const
{
  std::vector<std::string> lNames;
  BOOST_FOREACH( tMetricMap::value_type p, metrics_) {
    lNames.push_back( p.first );
  }
  return lNames;
}


AbstractMetric& MonitorableObject::getMetric( const std::string& aId )
{
  try {
    return *metrics_.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw MetricNotFoundInMonitorableObject("MonitorableObject \"" + this->path() + "\" does not contain metric of ID \"" + aId + "\"");
  }
}


StatusFlag MonitorableObject::getStatus() const
{
  StatusFlag result = kNoLimit;
  
  std::vector<std::string> childIds = getChildren();

  for(std::vector<std::string>::const_iterator it=childIds.begin(); it != childIds.end(); it++)
  {
    if(swatch::core::MonitorableObject* monObj = dynamic_cast<swatch::core::MonitorableObject*>(& this->getObj(*it)))
    {
      result = result & monObj->getStatus();
    }
  }
  
  BOOST_FOREACH( tMetricMap::value_type p, metrics_) {
    result = result & p.second->getValue().getStatus();
  }
  
  return result;
}


void MonitorableObject::updateMetrics() 
{
  timeval startTime;
  gettimeofday(&startTime, NULL);

  try{
    this->implementUpdateMetrics();
    
    // TODO: should lock a mutex ??
    updateErrorMsg_.clear();
  }
  catch(const std::exception& e)
  {
    // TODO: should lock a mutex ??
    updateErrorMsg_ = e.what();
  }

  BOOST_FOREACH(tMetricMap::value_type p, metrics_) {
    timeval lastUpdateTime = p.second->getUpdateTimestamp(); 
    if((lastUpdateTime.tv_sec == startTime.tv_sec) && (lastUpdateTime.tv_usec < startTime.tv_usec))
      p.second->setValueUnknown();
    else if(lastUpdateTime.tv_sec < startTime.tv_sec)
      p.second->setValueUnknown();
  }
  
}


}
}
