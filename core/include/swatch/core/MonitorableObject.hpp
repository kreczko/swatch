/**
 * @file    MonitorableObject.hpp
 * @author  Andy Rose
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_MONITORABLEOBJECT_HPP__
#define	__SWATCH_CORE_MONITORABLEOBJECT_HPP__

// STL headers
#include <set>
#include <string>

// BOOST headers
#include "boost/unordered_map.hpp"

// log4cplus headers
#include <log4cplus/logger.h>

// SWATCH headers
#include "swatch/core/exception.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Metric.hpp"
#include "swatch/core/MonitoringSetting.hpp"


namespace swatch {
namespace core {

class MetricUpdateGuard;
class AbstractMonitorableStatus;


//! An object that contains metrics and/or other monitorable objects; i.e. an object representing a resource whose status (Good/Warning/Error) can be monitored by reading various quantities
class MonitorableObject : public Object {
public:
  explicit MonitorableObject( const std::string& aId );

  virtual ~MonitorableObject();
    
  /**
    * Names of registered metrics.
    * @return metric names
    */
  std::vector< std::string > getMetrics() const;

  //! Retrieve metric with given ID
  const AbstractMetric& getMetric( const std::string& aId ) const;

  //! Retrieve metric with given ID
  AbstractMetric& getMetric( const std::string& aId );

  //! Get overall object status based on status flags of child Metrics and child MonitorableObjects; returns kNoLimit in case there are no metrics.
  StatusFlag getStatusFlag() const;
  
  //! Update values of this object's metrics
  void updateMetrics();

  //! Update values of this object's metrics
  void updateMetrics(const MetricUpdateGuard& aGuard);

  void setMonitoringStatus(const swatch::core::monitoring::Status aMonStatus);
  swatch::core::monitoring::Status getMonitoringStatus() const;

protected:

  /*!
   * register a metric of type swatch::core::Metric<DataType> , without any error or warning limits 
   * @param aId ID string of the metric
   * @returns A reference to the registered metric
   */
  template <typename DataType>
  Metric<DataType>& registerMetric(const std::string& aId);  

  /*!
   * register a metric of type swatch::core::Metric<DataType>
   * @param aId ID string of the metric
   * @param aErrorCondition Functor that is used to determine whether metric's value indicates an error; must inherit from swatch::core::MetricCondition<DataType>
   * @returns A reference to the registered metric
   */
  template <typename DataType, class ErrorCondition>
  Metric<DataType>& registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition);

  /*!
   * register a metric of type swatch::core::Metric<DataType>
   * @param aId ID string of the metric
   * @param aErrorCondition Functor that is used to determine whether metric's value indicates an error; must inherit from swatch::core::MetricCondition<DataType>
   * @param aWarnCondition Functor that is used to determine whether metric's value constitutes a warning; must inherit from swatch::core::MetricCondition<DataType>
   * @returns A reference to the registered metric
   */
  template <typename DataType, class ErrorCondition, class WarnCondition> 
  Metric<DataType>& registerMetric(const std::string& aId, const ErrorCondition& aErrorCondition, const WarnCondition& aWarnCondition);
  
  //! Set value of metric
  template <typename DataType>
  void setMetricValue(Metric<DataType>& metric, const DataType& value);
  
  //! Set error condition of metric
  template <typename DataType, class ConditionType>
  void setErrorCondition(Metric<DataType>& aMetric, const ConditionType& aErrorCondition);
  
  //! Set warning condition of metric
  template <typename DataType, class ConditionType>
  void setWarningCondition(Metric<DataType>& aMetric, const ConditionType& aWarningCondition);
  
  //! Set error & warning conditions of metric
  template <typename DataType, class ErrorConditionType, class WarnConditionType>
  void setConditions(Metric<DataType>& aMetric, const ErrorConditionType& aErrorCondition, const WarnConditionType& aWarnCondition);
  
  //! User-defined function that retrieves values of all the object's metrics from the hardware
  virtual void retrieveMetricValues() = 0;
  
  void addMonitorable(MonitorableObject* aMonObj);

  //! Sets this object's monitorable status pointer (used by MetricWriteGuard, in updateMetrics method)
  void setMonitorableStatus(AbstractMonitorableStatus& aStatus, log4cplus::Logger& aLogger);

private:
  typedef boost::unordered_map< std::string , AbstractMetric* > MetricMap_t;

  MetricMap_t mMetrics;

  //! Stores details in case error occurs when updating the metric values
  std::string mUpdateErrorMsg;
  swatch::core::monitoring::Status mMonitoringStatus;
  AbstractMonitorableStatus* mStatus;
  log4cplus::Logger mLogger;
 
  friend class MetricUpdateGuard;
};


DEFINE_SWATCH_EXCEPTION(MetricAlreadyExistsInMonitorableObject);
DEFINE_SWATCH_EXCEPTION(MetricNotFoundInMonitorableObject);


class MetricUpdateGuard : boost::noncopyable {
public:
  MetricUpdateGuard(MonitorableObject& aMonObj);
  ~MetricUpdateGuard();
  
  bool isCorrectGuard(const MonitorableObject& aMonObj) const;

private:
  AbstractMonitorableStatus& mObjStatus;
};


} // namespace core
} // namespace swatch

#include "swatch/core/MonitorableObject.hxx"

#endif	/* __SWATCH_CORE_MONITORABLEOBJECT_HPP__ */

