
#include "Metric.hpp"
#include "MonitorableObject.hpp"


namespace swatch {
namespace core {


template <typename DataType>
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId)
{
  if (metrics_.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \""+aId+"\" already exists in object of path \""+path()+"\"");

  Metric<DataType>* m = new Metric<DataType>();
  metrics_.insert( tMetricMap::value_type(aId, m) );
  return *m;
}


template <typename DataType, class ErrorCondition>
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ErrorCondition >::value) , "class ErrorCondition must be a descendant of MetricCondtion<DataType>" );

  if (metrics_.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \""+aId+"\" already exists in object of path \""+path()+"\"");

  Metric<DataType>* m = new Metric<DataType>( new ErrorCondition(aErrorCondition) );
  metrics_.insert( tMetricMap::value_type(aId, m) );
  return *m;
}


template <typename DataType, class ErrorCondition, class WarnCondition> 
Metric<DataType>& MonitorableObject::registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition, const WarnCondition& aWarnCondition)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, ErrorCondition >::value) , "class ErrorCondition must be a descendant of MetricCondtion<DataType>" );
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<MetricCondition<DataType>, WarnCondition >::value) , "class WarnCondition must be a descendant of MetricCondtion<DataType>" );
    
  if (metrics_.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \""+aId+"\" already exists in object of path \""+path()+"\"");

  Metric<DataType>* m = new Metric<DataType>( new ErrorCondition(aErrorCondition), new WarnCondition(aWarnCondition) );
  metrics_.insert( tMetricMap::value_type(aId, m) );
  return *m;
}


template <typename DataType>
void MonitorableObject::setMetricValue(Metric<DataType>& metric, const DataType& value)
{
  metric.setValue(value);
}


}
}
