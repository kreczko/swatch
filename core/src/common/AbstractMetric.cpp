/* 
 * File:   AbstractMetric.cpp
 * Author: Tom Williams
 * Date:   June 2015
 */

#include "swatch/core/AbstractMetric.hpp"


#include <iostream>


namespace swatch {
namespace core {


AbstractMetric::AbstractMetric() {
}


AbstractMetric::~AbstractMetric() {
}



AbstractMetricCondition::AbstractMetricCondition() {
}


AbstractMetricCondition::~AbstractMetricCondition() {
}



std::ostream& operator<<(std::ostream& aOut, const AbstractMetricCondition& aCondition) {
  aCondition.print(aOut);
  return aOut;
}



MetricSnapshot::MetricSnapshot(swatch::core::StatusFlag flag,
    const std::string& value, timeval updateTime,
    boost::shared_ptr<AbstractMetricCondition> errCond,
    boost::shared_ptr<AbstractMetricCondition> warnCond,
    swatch::core::monitoring::Status m_status) :
        flag_(flag),
        value_(value),
        updateTimestamp_(updateTime),
        errorCondition_(errCond),
        warnCondition_(warnCond),
        monitoringStatus_(m_status){
}


swatch::core::StatusFlag MetricSnapshot::getStatus() const {
  return flag_;
}


const std::string& MetricSnapshot::getValue() const {
  return value_;
}


timeval MetricSnapshot::getUpdateTimestamp() const {
  return updateTimestamp_;
}


const AbstractMetricCondition* MetricSnapshot::getWarningCondition() const {
  return warnCondition_.get();
}


const AbstractMetricCondition* MetricSnapshot::getErrorCondition() const {
  return errorCondition_.get();
}

swatch::core::monitoring::Status MetricSnapshot::getMonitoringStatus() const {
  return monitoringStatus_;
}


}
}
