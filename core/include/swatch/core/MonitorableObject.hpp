/**
 * @file    MonitorableObject.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Monitorables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_CORE_MonitorableObject_HPP__
#define	__SWATCH_CORE_MonitorableObject_HPP__

// SWATCH Headers
#include "swatch/core/exception.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Metric.hpp"

// STL Headers
#include <set>
#include <string>

// BOOST Headers
#include "boost/unordered_map.hpp"

namespace swatch {
namespace core {

//! An object that contains metrics and/or other monitorable objects
class MonitorableObject : public Object {
public:
  explicit MonitorableObject( const std::string& aId );

  MonitorableObject( const std::string& aId, const XParameterSet& aParams );
    
  virtual ~MonitorableObject();
    
  /**
    * List of names of stored Metric.
    * @return set of metric names
    */    
  std::vector< std::string > getMetrics() const;

  //! Retrieve metric with given ID
  AbstractMetric& getMetric( const std::string& aId );

  //! Get overall object status based on status flags of child Metrics and child MonitorableObjects; returns kGood in case there are no metrics.
  StatusFlag getStatus() const;

protected:

  /*!
   * register a metric of type swatch::core::Metric
   * @param aId ID string of the metric
   * @param aObj object instance used to retrieve new data values
   * @param aRetrieveValueFunc method used to retrieve new data values
   * @param aMinGoodValue Minimum value resulting in "GOOD" value of status flag (i.e. lower data values result in "ERROR" status flag)
   * @param aMaxGoodValue Maximum value resulting in "GOOD" value of status flag (i.e. higher data values result in "ERROR" status flag)
   */
  template <typename DataType, typename ObjectType> 
  void registerMetric( const std::string& aId, ObjectType& aObj, typename Metric<DataType, ObjectType>::ParentMemberFunctionPtr aRetrieveValueFunc, DataType aMinGoodValue, DataType aMaxGoodValue);
    
private:
  typedef boost::unordered_map< std::string , AbstractMetric* > tMonitorableMap;

  tMonitorableMap mMonitorables;
};

DEFINE_SWATCH_EXCEPTION(MonitorableAlreadyExistsInMonitorableObject);

DEFINE_SWATCH_EXCEPTION(MonitorableNotFoundInMonitorableObject);

} // namespace core
} // namespace swatch

#include "swatch/core/MonitorableObject.hxx"

#endif	/* __SWATCH_CORE_MonitorableObject_HPP__ */

