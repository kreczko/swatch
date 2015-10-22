
#include "swatch/core/MonitorableObject.hpp"


#include "boost/foreach.hpp"


using namespace std;

namespace swatch {
namespace core {

  
MonitorableObject::MonitorableObject( const std::string& aId ) :
    Object( aId ),
    metrics_(),
    updateErrorMsg_(""),
    monitoringStatus_(monitoring::kEnabled)
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


const AbstractMetric& MonitorableObject::getMetric( const std::string& aId ) const
{
  try {
    return *metrics_.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw MetricNotFoundInMonitorableObject("MonitorableObject \"" + getPath() + "\" does not contain metric of ID \"" + aId + "\"");
  }
}



AbstractMetric& MonitorableObject::getMetric( const std::string& aId )
{
  try {
    return *metrics_.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw MetricNotFoundInMonitorableObject("MonitorableObject \"" + getPath() + "\" does not contain metric of ID \"" + aId + "\"");
  }
}


StatusFlag MonitorableObject::getStatus() const
{
  StatusFlag result = kNoLimit;
  
  std::vector<std::string> childIds = getChildren();

  for(std::vector<std::string>::const_iterator it=childIds.begin(); it != childIds.end(); it++)
  {
    if(const swatch::core::MonitorableObject* monObj = dynamic_cast<const swatch::core::MonitorableObject*>(& this->getObj(*it)))
    {
      // only enabled children contribute to the status
      if (monObj->getMonitoringStatus() == monitoring::kEnabled)
        result = result & monObj->getStatus();
    }
  }
  
  BOOST_FOREACH( tMetricMap::value_type p, metrics_) {
    // only enabled metrics contribute to the status
    if (p.second->getValue().getMonitoringStatus() == monitoring::kEnabled)
      result = result & p.second->getValue().getStatus();
  }
  
  return result;
}


void MonitorableObject::updateMetrics() 
{
  timeval startTime;
  gettimeofday(&startTime, NULL);

  try{
    this->retrieveMetricValues();
    
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
    // last update before start time equals failure
    bool failedUpdate = timercmp(&lastUpdateTime, &startTime, <);
    bool isEnabled = p.second->getMonitoringStatus()
        != monitoring::kDisabled;
    // only set the value to unknown for enabled metrics
    if (failedUpdate && isEnabled)
      p.second->setValueUnknown();
  }
  
}

void MonitorableObject::setMonitoringStatus(const swatch::core::monitoring::Status m_status){
  monitoringStatus_ = m_status;
}

swatch::core::monitoring::Status
MonitorableObject::getMonitoringStatus() const {
  return monitoringStatus_;
}


}
}
