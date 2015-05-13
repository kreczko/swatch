/**
 * @file    MonitorableObject.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Monitorables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_TEST_MonitorableObject_HPP__
#define	__SWATCH_TEST_MonitorableObject_HPP__

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

class MonitorableObject : public Object {
public:
  explicit MonitorableObject( const std::string& aId );

  MonitorableObject( const std::string& aId, const XParameterSet& aParams );
    
  virtual ~MonitorableObject();
    
  /**
    * List of Configuration Sequence names stored.
    * @return set of command names
    */    
  std::set< std::string > getMonitorables() const;


  Metric* getMonitorable( const std::string& aId );

  template < typename T >
  void registerMonitorable( const std::string& aId );

  void registerMonitorable( const std::string& aId , Metric* aMetric);

  typedef boost::unordered_map< std::string , Metric* > tMonitorableMap;
     
protected:

private:
  tMonitorableMap mMonitorables;
};

DEFINE_SWATCH_EXCEPTION(MonitorableAlreadyExistsInMonitorableObject);

DEFINE_SWATCH_EXCEPTION(MonitorableNotFoundInMonitorableObject);
} // namespace core
} // namespace swatch

#include "swatch/core/MonitorableObject.hxx"

#endif	/* __SWATCH_TEST_MonitorableObject_HPP__ */

