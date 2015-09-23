/**
 * @file    Metric.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_METRIC_HPP__
#define __SWATCH_CORE_METRIC_HPP__


#include <string>

#include "boost/thread/mutex.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/shared_ptr.hpp"

#include "swatch/core/AbstractMetric.hpp"


namespace swatch {
namespace core {

class MonitorableObject;

template<typename DataType>
class MetricCondition;

/**
 * Metric encapsulating monitoring data of given type
 * 
 * @tparam DataType type of the monitoring data
 */
template<typename DataType>
class Metric : public AbstractMetric {
    
public:

    //! Construct a metric with no error/warning limits
    Metric();
    
    /*!
     * Construct the Metric 
     * @param aErrorCondition Functor used to determine if the metric's value indicates an error; the Metric takes ownership of this condition object. NULL value represents no error condition.
     * @param aWarningCondition Functor used to determine if the metric's value indicates a warning; the Metric takes ownership of this condition object. NULL value represents no warning condition.
     */
    Metric(MetricCondition<DataType>* aErrorCondition, MetricCondition<DataType>* aWarnCondition = NULL);

    ~Metric();

    MetricSnapshot getValue() const;
    
    //! Returns time at which metric's value was last updated
    timeval getUpdateTimestamp() const;

    MonitoringStatus getMonitoringStatus() const;
    void setMonitoringStatus(MonitoringStatus status);

protected:

    //! Set the value of the metric
    void setValue(const DataType& value);
    
private:
    //! Set the value of the metric to being unknown
    void setValueUnknown();
    
    //! Latest retrieved value of metric; set to NULL if metric value is unknown.
    boost::scoped_ptr<DataType> value_;   

    timeval updateTimestamp_;
    
    // TODO: Maybe eventually update to read-write mutex if needed ???
    //! Mutex used to stop corruption of value_
    mutable boost::mutex mutex_;

    boost::shared_ptr<MetricCondition<DataType> > errorCondition_;
    boost::shared_ptr<MetricCondition<DataType> > warnCondition_;
    
    friend class MonitorableObject;

    swatch::core::MonitoringStatus monitoringStatus_;
};


//! Function template for representing metric data as string 
template<typename DataType>
std::string convertMetricDataToString(DataType data);

template<>
std::string convertMetricDataToString<bool>(bool data);


/**
 * Metric condition for monitoring data of given type
 * 
 * @tparam DataType type of the monitoring data
 */
template<typename DataType>
class MetricCondition : public AbstractMetricCondition {
public:
     MetricCondition();
     
     virtual ~MetricCondition();
     
     virtual bool operator()(const DataType&) const = 0;
};


} // namespace core
} // namespace swatch



#include "Metric.hxx"

#endif	/* __SWATCH_CORE_METRIC_HPP__ */

