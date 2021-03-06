
#ifndef __SWATCH_CORE_MONITORABLEOBJECT_HXX__
#define	__SWATCH_CORE_MONITORABLEOBJECT_HXX__


#include "Metric.hpp"
#include "MonitorableObject.hpp"


namespace swatch {
namespace core {


template <typename DataType>
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId)
{
  if (mMetrics.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \"" + aId + "\" already exists in object of path \"" + getPath() + "\"");

  Metric<DataType>* m = new Metric<DataType>();
  mMetrics.insert( MetricMap_t::value_type(aId, m) );
  return *m;
}


template <typename DataType, class ErrorCondition>
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ErrorCondition >::value) , "class ErrorCondition must be a descendant of MetricCondtion<DataType>" );

  if (mMetrics.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \"" + aId + "\" already exists in object of path \"" + getPath() + "\"");

  Metric<DataType>* m = new Metric<DataType>( new ErrorCondition(aErrorCondition) );
  mMetrics.insert( MetricMap_t::value_type(aId, m) );
  return *m;
}


template <typename DataType, class ErrorCondition, class WarnCondition> 
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition, const WarnCondition& aWarnCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ErrorCondition >::value) , "class ErrorCondition must be a descendant of MetricCondtion<DataType>" );
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, WarnCondition >::value) , "class WarnCondition must be a descendant of MetricCondtion<DataType>" );
    
  if (mMetrics.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \"" + aId + "\" already exists in object of path \"" + getPath() + "\"");

  Metric<DataType>* m = new Metric<DataType>( new ErrorCondition(aErrorCondition), new WarnCondition(aWarnCondition) );
  mMetrics.insert( MetricMap_t::value_type(aId, m) );
  return *m;
}


template <typename DataType>
void MonitorableObject::setMetricValue(Metric<DataType>& aMetric, const DataType& aValue)
{
  aMetric.setValue(aValue);
}


template <typename DataType, class ConditionType>
void MonitorableObject::setErrorCondition(Metric<DataType>& aMetric, const ConditionType& aErrorCondition)
{
  aMetric.setErrorCondition(aErrorCondition);
}


template <typename DataType, class ConditionType>
void MonitorableObject::setWarningCondition(Metric<DataType>& aMetric, const ConditionType& aWarningCondition)
{
  aMetric.setWarningCondition(aWarningCondition);
}


template <typename DataType, class ErrorConditionType, class WarnConditionType>
void MonitorableObject::setConditions(Metric<DataType>& aMetric, const ErrorConditionType& aErrorCondition, const WarnConditionType& aWarnCondition)
{
  aMetric.setErrorCondition(aErrorCondition);
  aMetric.setWarningCondition(aWarnCondition);
}

}
}

#endif	/* __SWATCH_CORE_MONITORABLEOBJECT_HXX__ */
