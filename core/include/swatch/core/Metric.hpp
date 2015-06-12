/**
 * @file    Metric.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_METRIC_HPP__
#define	__SWATCH_CORE_METRIC_HPP__


#include <string>
#include <sys/time.h>

#include "boost/thread/mutex.hpp"
#include "boost/scoped_ptr.hpp"

#include "swatch/core/StatusFlag.hpp"


namespace swatch {
namespace core {

class MonitorableObject;

/*!
 * @brief Represents some set of monitoring data that is retrieved from hardware.
 * 
 * This class is intended to contain the value of some monitoring data, providing
 * an interface to update the stored data value, and an interface to access that 
 * stored data value, along with a corresponding status flag that expresses whether
 * or not the value lies within some internally-stored limits
 */
class AbstractMetric{
public:
    AbstractMetric();
    
    virtual ~AbstractMetric();

    // TODO: Make const ??
    //! Returns status flag deduced from comparing the stored value with limits, as well as string representation of the metric's value. (THREAD SAFE)
    virtual std::pair<swatch::core::StatusFlag, std::string> getValue() = 0;
    
    //! Returns time at which metric's value was last updated
    virtual timeval getUpdateTimestamp() = 0;

    
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

template<typename DataType>
class Metric : public AbstractMetric {
    
public:

    /*!
     * Construct the Metric 
     * @param minGoodValue Minimum value resulting in "GOOD" value of status flag (i.e. lower data values result in "ERROR" status flag)
     * @param maxGoodValue Maximum value resulting in "GOOD" value of status flag (i.e. higher data values result in "ERROR" status flag)
     */
    Metric(DataType minGoodValue, DataType maxGoodValue);

    ~Metric();
    
    std::pair<swatch::core::StatusFlag, std::string> getValue();
    
    timeval getUpdateTimestamp();
    
private:

    //! Set the value of the metric
    void setValue(const DataType& value);
    
    //! Set the value of the metric to being unknown
    void setValueUnknown();
    
    //! Latest retrieved value of metric; set to NULL if metric value is unknown.
    boost::scoped_ptr<DataType> value_;   

    timeval updateTimestamp_;
    
    // TODO: Maybe eventually update to read-write mutex if needed ???
    //! Mutex used to stop corruption of data_ and updateErrorMsg_
    boost::mutex mutex_;

    // TODO: Update limits to being more flexible, and settable from base class
    
    DataType minGoodValue_;
    DataType maxGoodValue_;
    
    friend class MonitorableObject;
 };

//! Function template for representing metric data as string 
template<typename DataType>
std::string convertMetricDataToString(DataType data);

template<>
std::string convertMetricDataToString<bool>(bool data);


} // namespace core
} // namespace swatch



#include "Metric.hxx"

#endif	/* __SWATCH_CORE_METRIC_HPP__ */

