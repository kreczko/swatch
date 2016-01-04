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
  LOG(swatch::logger::kDebug) << "Creating MetricView from object " << aMonObject.getPath() << " with regex " << aRegex;
  
  swatch::core::Object::iterator objIt;
  for(objIt = aMonObject.begin(); objIt != aMonObject.end(); objIt++)
  {
    if(swatch::core::MonitorableObject* monObject = dynamic_cast<swatch::core::MonitorableObject*>(&*objIt))
    {
      std::vector<std::string> metrics = monObject->getMetrics();
      
      BOOST_FOREACH(const std::string& metricId, metrics)
      {
        const std::string metricPath(monObject->getPath() + "." + metricId);
        
        // Check against regex
        boost::cmatch match;
        if ( boost::regex_match (metricPath.c_str(), match, boost::regex(aRegex)))
        {
          LOG(swatch::logger::kDebug) << "MetricView : metric with path \"" << metricPath << "\" matches regex";
          mMetricsMap[metricPath] = &(monObject->getMetric(metricId));
          mMonitorableObjects.insert(monObject);
        }
      }
    }
  }
}


MetricView::~MetricView() {
}


void MetricView::update() {
  BOOST_FOREACH(std::set<swatch::core::MonitorableObject*>::value_type monObj, mMonitorableObjects)
  {
    monObj->updateMetrics();
  }
}
    

size_t MetricView::size() const {
  return mMetricsMap.size();
}


MetricView::iterator MetricView::begin() {
  return mMetricsMap.begin();
}


MetricView::iterator MetricView::end() {
  return mMetricsMap.end();
}


MetricView::const_iterator MetricView::begin() const {
  return mMetricsMap.begin();
}


MetricView::const_iterator MetricView::end() const {
  return mMetricsMap.end();
}


}
}
