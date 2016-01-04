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
  mValue(NULL),
  mUpdateTimestamp(),
  mErrorCondition(),
  mWarnCondition(),
  mMonitoringStatus(monitoring::kEnabled)
{
}


template<typename DataType>
Metric<DataType>::Metric(MetricCondition<DataType>* aErrorCondition,
    MetricCondition<DataType>* aWarnCondition) :
        mValue(NULL),
        mUpdateTimestamp(),
        mErrorCondition(aErrorCondition),
        mWarnCondition(aWarnCondition),
        mMonitoringStatus(monitoring::kEnabled) {
}


template<typename DataType>
Metric<DataType>::~Metric(){
}


template<typename DataType>
MetricSnapshot Metric<DataType>::getSnapshot() const {
    boost::lock_guard<boost::mutex> lock(mMutex);

    swatch::core::StatusFlag flag = kUnknown;
    std::string value = boost::lexical_cast<std::string>(swatch::core::kUnknown);
    
    if (this->mValue != NULL)
    {
    if ((mErrorCondition == NULL) && (mWarnCondition == NULL))
            flag = kNoLimit;
        else if ( mErrorCondition && (*mErrorCondition)(*mValue))
            flag = kError;
        else if ( mWarnCondition && (*mWarnCondition)(*mValue))
            flag = kWarning;
        else
            flag = kGood;

        value = convertMetricDataToString(*mValue);
    }
    
    if (mMonitoringStatus == monitoring::kDisabled)
      flag = kNoLimit; //disabled metrics always return kNoLimit

    return MetricSnapshot(flag, value, mUpdateTimestamp, mErrorCondition, mWarnCondition, mMonitoringStatus);
}


template<typename DataType>
timeval Metric<DataType>::getUpdateTimestamp() const
{
    boost::lock_guard<boost::mutex> lock(mMutex);
    return mUpdateTimestamp;
}


template<typename DataType>
void Metric<DataType>::setValue(const DataType& aValue) {
    boost::lock_guard<boost::mutex> lock(mMutex);
    gettimeofday(&mUpdateTimestamp, NULL);
    mValue.reset(new DataType(aValue));
}


template<typename DataType>
void Metric<DataType>::setValueUnknown() {
    boost::lock_guard<boost::mutex> lock(mMutex);
    gettimeofday(&mUpdateTimestamp, NULL);
    mValue.reset(NULL);
}

template<typename DataType>
template<class ConditionType>
void Metric<DataType>::setErrorCondition(const ConditionType& aErrorCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ConditionType >::value) , "class ConditionType must be a descendant of MetricCondtion<DataType>" );

  boost::lock_guard<boost::mutex> lLock(mMutex);
  mErrorCondition.reset(new ConditionType(aErrorCondition));
}


template<typename DataType>
template<class ConditionType>
void Metric<DataType>::setWarningCondition(const ConditionType& aWarningCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ConditionType >::value) , "class ConditionType must be a descendant of MetricCondtion<DataType>" );

  boost::lock_guard<boost::mutex> lLock(mMutex);
  mWarnCondition.reset(new ConditionType(aWarningCondition));
}


template<typename DataType>
monitoring::Status Metric<DataType>::getMonitoringStatus() const{
  boost::lock_guard<boost::mutex> lock(mMutex);
  return mMonitoringStatus;
}

template<typename DataType>
void Metric<DataType>::setMonitoringStatus(monitoring::Status aMonStatus){
  boost::lock_guard<boost::mutex> lock(mMutex);
  mMonitoringStatus = aMonStatus;
}

template<typename DataType>
std::string convertMetricDataToString(DataType aData)
{
    return boost::lexical_cast<std::string>(aData);
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

