/**
 * @file    AbstractMetric.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_ABSTRACTMETRIC_HPP__
#define __SWATCH_CORE_ABSTRACTMETRIC_HPP__


#include <string>
#include <sys/time.h>
#include <iosfwd>
#include <map>

#include "boost/shared_ptr.hpp"

#include "swatch/core/StatusFlag.hpp"
#include "swatch/core/MonitoringSetting.hpp"


namespace swatch {
namespace core {

class AbstractMetricCondition;
class MetricSnapshot;
class MonitorableObject;

/*!
 * @brief Represents some set of monitoring data that is retrieved from hardware.
 * 
 * This class is intended to contain the value of some monitoring data, provide
 * an interface to update the stored data value, and an interface to access that 
 * stored data value, along with a corresponding status flag that expresses whether
 * or not the value lies within some internally-stored limits
 */
class AbstractMetric{
public:
    AbstractMetric();
    
    virtual ~AbstractMetric();

    //! Returns metric value, status flag, and error/warning conditions, within a swatch::core::MetricSnapshot object. (THREAD SAFE)
    virtual MetricSnapshot getSnapshot() const = 0;
    
    //! Returns time at which metric's value was last updated. (THREAD SAFE)
    virtual timeval getUpdateTimestamp() const = 0;

    //! Returns the monitoring status as defined in enum MonitoringStatus
    virtual monitoring::Status getMonitoringStatus() const = 0;
    virtual void setMonitoringStatus(monitoring::Status aMonStatus) = 0;

    
protected:
    
    //! Set the value of the metric to being unknown
    virtual void setValueUnknown() = 0;

    friend class MonitorableObject;
};


std::ostream& operator<<(std::ostream& aOut, const AbstractMetricCondition& aCondition);


//! Base class for error/warning conditions stored in metric classes (that inherit from AbstractMetric).
class AbstractMetricCondition {
public:
    AbstractMetricCondition();
    virtual ~AbstractMetricCondition();
    
protected:
    virtual void print(std::ostream& aStream) const = 0;
    
    friend std::ostream& operator<<(std::ostream& aOut, const AbstractMetricCondition& aCondition);
};


class MetricSnapshot {
public:
  MetricSnapshot(swatch::core::StatusFlag , const std::string&, timeval,
      boost::shared_ptr<AbstractMetricCondition> aErrCond,
      boost::shared_ptr<AbstractMetricCondition> aWarnCond,
      swatch::core::monitoring::Status aMonStatus = monitoring::kEnabled);

    //! Returns status flag deduced from comparing the stored value with limits
    swatch::core::StatusFlag getStatusFlag() const;
    
    //! Returns string representation of the metric's value
    const std::string& getValue() const;
    
    //! Returns time at which metric's value was last updated
    timeval getUpdateTimestamp() const;
    
    //! Returns metric's warning condition; NULL returned if metric doesn't have any warning condition
    const AbstractMetricCondition* getWarningCondition() const;
    
    //! Returns metric's error condition; NULL returned if metric doesn't have any error condition
    const AbstractMetricCondition* getErrorCondition() const;
    
    swatch::core::monitoring::Status getMonitoringStatus() const;

private:
    swatch::core::StatusFlag mFlag;
    std::string mValue; //TODO ??? Update to boost shared_ptr to actual data value (without templating class, just templating CTOR) ???
    timeval mUpdateTimestamp;
    boost::shared_ptr<AbstractMetricCondition> mErrorCondition;
    boost::shared_ptr<AbstractMetricCondition> mWarnCondition;
    swatch::core::monitoring::Status mMonitoringStatus;
};


} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_ABSTRACTMETRIC_HPP__ */
