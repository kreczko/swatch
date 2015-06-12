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
Metric<DataType>::Metric(DataType minGoodValue, DataType maxGoodValue) : 
  value_(NULL),
  minGoodValue_(minGoodValue),
  maxGoodValue_(maxGoodValue)
{
    
}


template<typename DataType>
Metric<DataType>::~Metric(){
}


template<typename DataType>
std::pair<swatch::core::StatusFlag, std::string> Metric<DataType>::getValue() {
    boost::lock_guard<boost::mutex> lock(mutex_);

    if(this->value_ == NULL)
    {
        return std::pair<swatch::core::StatusFlag, std::string>(kUnknown, boost::lexical_cast<std::string>(swatch::core::kUnknown));
    }
    
    std::pair<swatch::core::StatusFlag, std::string> result;
    if ( ( *(this->value_) <= this->maxGoodValue_ ) && ( *(this->value_) >= this->minGoodValue_ ))
        result.first = kGood;
    else
        result.first = kError;
    
    result.second = convertMetricDataToString(*this->value_);
    
    return result;
}

template<typename DataType>
timeval Metric<DataType>::getUpdateTimestamp()
{
    boost::lock_guard<boost::mutex> lock(mutex_);
    return updateTimestamp_;
}

/*
template<typename DataType, class ParentObjectType>
void Metric<DataType, ParentObjectType>::update() {

    try{
        DataType newValue = (obj_.*fRetrieveValue_)();

        // For efficiency, only lock the mutex after the metric value has been retrieved (potentially long operation)
        boost::lock_guard<boost::mutex> lock(mutex_);
        value_.reset(new DataType(newValue));
        updateErrorMsg_.clear();
    }
    catch(const std::exception& e)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        value_.reset(NULL);
        updateErrorMsg_ = e.what();
    }
}
*/

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

} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_METRIC_HPP__ */

