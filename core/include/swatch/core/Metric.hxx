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



namespace swatch {
namespace core {


template<typename DataType, class ParentObjectType>
Metric<DataType, ParentObjectType>::Metric(ParentObjectType& parent, ParentMemberFunctionPtr fRetrieveValue, DataType minGoodValue, DataType maxGoodValue) : 
  obj_(parent),
  fRetrieveValue_(fRetrieveValue),
  value_(NULL),
  minGoodValue_(minGoodValue),
  maxGoodValue_(maxGoodValue)
{
    
}


template<typename DataType, class ParentObjectType>
Metric<DataType, ParentObjectType>::~Metric(){
}


template<typename DataType, class ParentObjectType>
std::pair<swatch::core::StatusFlag, std::string> Metric<DataType, ParentObjectType>::getValue() {
    boost::lock_guard<boost::mutex> lock(mutex_);

    if(this->value_ == NULL)
    {
        return std::pair<swatch::core::StatusFlag, std::string>(kUnknown, updateErrorMsg_);
    }
    
    std::pair<swatch::core::StatusFlag, std::string> result;
    if ( ( *(this->value_) <= this->maxGoodValue_ ) && ( *(this->value_) >= this->minGoodValue_ ))
        result.first = kGood;
    else
        result.first = kError;
    
    result.second = convertMetricDataToString(*this->value_);
    
    return result;
}


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


template<typename DataType>
std::string convertMetricDataToString(DataType data)
{
    return boost::lexical_cast<std::string>(data);
}

} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_METRIC_HPP__ */

