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
        mFlag(flag),
        mValue(value),
        mUpdateTimestamp(updateTime),
        mErrorCondition(errCond),
        mWarnCondition(warnCond),
        mMonitoringStatus(m_status){
}


swatch::core::StatusFlag MetricSnapshot::getStatusFlag() const {
  return mFlag;
}


const std::string& MetricSnapshot::getValue() const {
  return mValue;
}


timeval MetricSnapshot::getUpdateTimestamp() const {
  return mUpdateTimestamp;
}


const AbstractMetricCondition* MetricSnapshot::getWarningCondition() const {
  return mWarnCondition.get();
}


const AbstractMetricCondition* MetricSnapshot::getErrorCondition() const {
  return mErrorCondition.get();
}

swatch::core::monitoring::Status MetricSnapshot::getMonitoringStatus() const {
  return mMonitoringStatus;
}


}
}
