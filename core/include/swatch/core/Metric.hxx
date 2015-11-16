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


template<typename DataType>
Metric<DataType>::Metric() :
  value_(NULL),
  updateTimestamp_(),
  errorCondition_(),
  warnCondition_(),
  monitoringStatus_(monitoring::kEnabled)
{
}


template<typename DataType>
Metric<DataType>::Metric(MetricCondition<DataType>* aErrorCondition,
    MetricCondition<DataType>* aWarnCondition) :
        value_(NULL),
        updateTimestamp_(),
        errorCondition_(aErrorCondition),
        warnCondition_(aWarnCondition),
        monitoringStatus_(monitoring::kEnabled) {
}


template<typename DataType>
Metric<DataType>::~Metric(){
}


template<typename DataType>
MetricSnapshot Metric<DataType>::getSnapshot() const {
    boost::lock_guard<boost::mutex> lock(mutex_);

    swatch::core::StatusFlag flag = kUnknown;
    std::string value = boost::lexical_cast<std::string>(swatch::core::kUnknown);
    
    if (this->value_ != NULL)
    {
    if ((errorCondition_ == NULL) && (warnCondition_ == NULL))
            flag = kNoLimit;
        else if ( errorCondition_ && (*errorCondition_)(*value_))
            flag = kError;
        else if ( warnCondition_ && (*warnCondition_)(*value_))
            flag = kWarning;
        else
            flag = kGood;

        value = convertMetricDataToString(*value_);
    }
    
    if (monitoringStatus_ == monitoring::kDisabled)
      flag = kNoLimit; //disabled metrics always return kNoLimit

    return MetricSnapshot(flag, value, updateTimestamp_, errorCondition_, warnCondition_, monitoringStatus_);
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
template<class ConditionType>
void Metric<DataType>::setErrorCondition(const ConditionType& aErrorCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ConditionType >::value) , "class ConditionType must be a descendant of MetricCondtion<DataType>" );

  boost::lock_guard<boost::mutex> lLock(mutex_);
  errorCondition_.reset(new ConditionType(aErrorCondition));
}


template<typename DataType>
template<class ConditionType>
void Metric<DataType>::setWarningCondition(const ConditionType& aWarningCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ConditionType >::value) , "class ConditionType must be a descendant of MetricCondtion<DataType>" );

  boost::lock_guard<boost::mutex> lLock(mutex_);
  warnCondition_.reset(new ConditionType(aWarningCondition));
}


template<typename DataType>
monitoring::Status Metric<DataType>::getMonitoringStatus() const{
  boost::lock_guard<boost::mutex> lock(mutex_);
  return monitoringStatus_;
}

template<typename DataType>
void Metric<DataType>::setMonitoringStatus(monitoring::Status status){
  boost::lock_guard<boost::mutex> lock(mutex_);
  monitoringStatus_ = status;
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

