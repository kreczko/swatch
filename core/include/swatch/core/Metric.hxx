/**
 * @file    Metric.hxx
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_METRIC_HXX__
#define	__SWATCH_CORE_METRIC_HXX__


#include <exception>

#include "boost/lexical_cast.hpp"
#include "boost/thread/lock_guard.hpp"
#include "Metric.hpp"



namespace swatch {
namespace core {


template<typename DataType>
Metric<DataType>::Metric() :
  value_(NULL)
{
}


template<typename DataType>
Metric<DataType>::Metric(MetricCondition<DataType>* aErrorCondition, MetricCondition<DataType>* aWarnCondition) : 
  value_(NULL),
  errorCondition_(aErrorCondition),
  warnCondition_(aWarnCondition)
{
}


template<typename DataType>
Metric<DataType>::~Metric(){
}


template<typename DataType>
MetricSnapshot Metric<DataType>::getValue() const {
    boost::lock_guard<boost::mutex> lock(mutex_);

    swatch::core::StatusFlag flag = kUnknown;
    std::string value = boost::lexical_cast<std::string>(swatch::core::kUnknown);
    
    if (this->value_ != NULL)
    {
        if ( (errorCondition_ == NULL) && (warnCondition_ == NULL) )
            flag = kNoLimit;
        else if ( errorCondition_ && (*errorCondition_)(*value_))
            flag = kError;
        else if ( warnCondition_ && (*warnCondition_)(*value_))
            flag = kWarning;
        else
            flag = kGood;

        value = convertMetricDataToString(*value_);
    }
    
    return MetricSnapshot(flag, value, updateTimestamp_, errorCondition_, warnCondition_);
}


template<typename DataType>
timeval Metric<DataType>::getUpdateTimestamp() const
{
    boost::lock_guard<boost::mutex> lock(mutex_);
    return updateTimestamp_;
}


template<typename DataType>
void Metric<DataType>::setValue(const DataType& value) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    gettimeofday(&updateTimestamp_, NULL);
    value_.reset(new DataType(value));
}


template<typename DataType>
void Metric<DataType>::setValueUnknown() {
    boost::lock_guard<boost::mutex> lock(mutex_);
    gettimeofday(&updateTimestamp_, NULL);
    value_.reset(NULL);
}


template<typename DataType>
std::string convertMetricDataToString(DataType data)
{
    return boost::lexical_cast<std::string>(data);
}


template<typename DataType>
MetricCondition<DataType>::MetricCondition() {
}


template<typename DataType>
MetricCondition<DataType>::~MetricCondition() {
}


} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_METRIC_HPP__ */

