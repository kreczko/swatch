/* 
 * File:   MetricView.cpp
 * Author: Tom Williams
 * Date: May 2015
 */

#include "swatch/core/MetricView.hpp"

// BOOST headers
#include "boost/foreach.hpp"
#include "boost/regex.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Metric.hpp"
#include "swatch/core/MonitorableObject.hpp"




namespace swatch {
namespace core {

MetricView::MetricView(swatch::core::MonitorableObject& aMonObject, const std::string& aRegex) {
  LOG(swatch::logger::kNotice) << "Creating MetricView from object " << aMonObject.path() << " with regex " << aRegex;
  
  swatch::core::Object::iterator objIt;
  for(objIt = aMonObject.begin(); objIt != aMonObject.end(); objIt++)
  {
    if(swatch::core::MonitorableObject* monObject = dynamic_cast<swatch::core::MonitorableObject*>(&*objIt))
    {
      std::vector<std::string> metrics = monObject->getMetrics();
      
      BOOST_FOREACH(const std::string& metricId, metrics)
      {
        const std::string metricPath(monObject->path() + "." + metricId);
        
        // Check against regex
        boost::cmatch match;
        if ( boost::regex_match (metricPath.c_str(), match, boost::regex(aRegex)))
        {
          LOG(swatch::logger::kNotice) << "MetricView : metric with path \"" << metricPath << "\" matches regex";
          metricsMap_[metricPath] = &(monObject->getMetric(metricId));
          monitorableObjects_.insert(monObject);
        }
      }
    }
  }
}


MetricView::~MetricView() {
}


void MetricView::update() {
  BOOST_FOREACH(std::set<swatch::core::MonitorableObject*>::value_type monObj, monitorableObjects_)
  {
    monObj->updateMetrics();
  }
}
    

size_t MetricView::size() const {
  return metricsMap_.size();
}


MetricView::iterator MetricView::begin() {
  return metricsMap_.begin();
}


MetricView::iterator MetricView::end() {
  return metricsMap_.end();
}


MetricView::const_iterator MetricView::begin() const {
  return metricsMap_.begin();
}


MetricView::const_iterator MetricView::end() const {
  return metricsMap_.end();
}


}
}
