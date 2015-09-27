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

#include "boost/shared_ptr.hpp"

#include "swatch/core/StatusFlag.hpp"


namespace swatch {
namespace core {

class AbstractMetricCondition;
class MetricSnapshot;
class MonitorableObject;

/**
 * Flag for deciding if failures of MonitorableObject will affect the parent.
 * If set to kENABLED they will (default behaviour) and if set to kNON_CRITICAL
 * they will not.
 */
namespace monitoring {
enum Status {
  kEnabled,
  kNonCritical,
  kDisabled,
};
}


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
    virtual MetricSnapshot getValue() const = 0;
    
    //! Returns time at which metric's value was last updated. (THREAD SAFE)
    virtual timeval getUpdateTimestamp() const = 0;

    //! Returns the monitoring status as defined in enum MonitoringStatus
    virtual monitoring::Status getMonitoringStatus() const = 0;

    
protected:
    
    //! Set the value of the metric to being unknown
    virtual void setValueUnknown() = 0;

    friend class MonitorableObject;
    
    /* Possibilities for the future: Masking, and updating limits at runtime
    bool isMasked() const;
    
    void mask(bool setMasked) const;
    
    //MAYBE: void setLimits(const std::string& ) const; 
    */
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
  MetricSnapshot(swatch::core::StatusFlag, const std::string&, timeval,
      boost::shared_ptr<AbstractMetricCondition>,
      boost::shared_ptr<AbstractMetricCondition>,
      swatch::core::monitoring::Status m_status = monitoring::kEnabled);

    //! Returns status flag deduced from comparing the stored value with limits
    swatch::core::StatusFlag getStatus() const;
    
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
    const swatch::core::StatusFlag flag_;
    const std::string value_; //TODO ??? Update to boost shared_ptr to actual data value (without templating class, just templating CTOR) ???
    const timeval updateTimestamp_;
    const boost::shared_ptr<AbstractMetricCondition> errorCondition_;
    const boost::shared_ptr<AbstractMetricCondition> warnCondition_;
    const swatch::core::monitoring::Status monitoringStatus_;
};


} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_ABSTRACTMETRIC_HPP__ */
