/**
 * @file    Metric.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_METRIC_HPP__
#define	__SWATCH_CORE_METRIC_HPP__


#include <string>

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
    
    //! Retrieve the value of the metric from the hardware, and update the stored value. (THREAD SAFE)
    virtual void update() = 0;
    
    /* Possibilities for the future: Masking, and updating limits at runtime
    bool isMasked() const;
    
    void mask(bool setMasked) const;
    
    //MAYBE: void setLimits(const std::string& ) const; 
    */
};

template<typename DataType, class ParentObjectType>
class Metric : public AbstractMetric {

// TODO: ?? Assert that ParentObjectType is derived from swatch::core::MonitorableObject ??
    
public:
    //! C++ type of member function used to retrieve the metric value from hardware
    typedef DataType (ParentObjectType::*ParentMemberFunctionPtr)() const; 
    
    /*!
     * Construct a Metric that retrieves the data via a member function pointer
     * @param parent object instance used to retrieve new data values
     * @param fRetrieveValue method used to retrieve new data values
     * @param minGoodValue Minimum value resulting in "GOOD" value of status flag (i.e. lower data values result in "ERROR" status flag)
     * @param maxGoodValue Maximum value resulting in "GOOD" value of status flag (i.e. higher data values result in "ERROR" status flag)
     */
    Metric(ParentObjectType& parent, ParentMemberFunctionPtr fRetrieveValue, DataType minGoodValue, DataType maxGoodValue);

    ~Metric();
    
    std::pair<swatch::core::StatusFlag, std::string> getValue();
    
    virtual void update();
    
private:
    
    //! Reference to the object instance that owns this metric; used when retrieving the metric value from the hardware.
    ParentObjectType& obj_;
    
    //! Member function used to retrieve the metric value from hardware.
    const ParentMemberFunctionPtr fRetrieveValue_;
    
    //! Latest retrieved value of metric; set to NULL if metric value is unknown.
    boost::scoped_ptr<DataType> value_;   

    // TODO: Maybe eventually update to read-write mutex if needed ???
    //! Mutex used to stop corruption of data_ and updateErrorMsg_
    boost::mutex mutex_;
    
    //! Stores details in case error occurs when updating the metric value
    std::string updateErrorMsg_;
    
    // TODO: Update limits to being more flexible, and settable from base class
    
    DataType minGoodValue_;
    DataType maxGoodValue_;
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

