
#include "Metric.hpp"


namespace swatch {
namespace core {

template <typename DataType> 
Metric<DataType>& MonitorableObject::registerMetric( const std::string& aId, DataType aMinGoodValue, DataType aMaxGoodValue)
{
  if (metrics_.count(aId))
    throw MetricAlreadyExistsInMonitorableObject("Metric of ID \""+aId+"\" already exists in object of path \""+path()+"\"");

  Metric<DataType>* m = new Metric<DataType>(aMinGoodValue, aMaxGoodValue);
  metrics_.insert( tMetricMap::value_type(aId , m) );
  return *m;
}

template <typename DataType>
void MonitorableObject::setMetricValue(Metric<DataType>& metric, const DataType& value)
{
  metric.setValue(value);
}




}
}
