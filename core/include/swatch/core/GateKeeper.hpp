/**
 * @file    GateKeeper.hpp
 * @author  arose
 * @brief   Guardian to the underworld that is the online database
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_GATEKEEPER_HPP__
#define __SWATCH_CORE_GATEKEEPER_HPP__

// C++ Headers
#include <string>
#include <iostream>

// SWATCH headers
#include "swatch/core/Object.hpp"
#include "swatch/core/exception.hpp"
#include "swatch/core/AbstractMetric.hpp"
#include "swatch/core/MonitoringSetting.hpp"
#include <xdata/Serializable.h>

// boost headers
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"


namespace swatch {
namespace core {

class GateKeeper;
std::ostream& operator<< ( std::ostream& aStr , const swatch::core::GateKeeper& aGateKeeper );


class GateKeeper
{
friend std::ostream& operator<< ( std::ostream& aStr , const swatch::core::GateKeeper& aGateKeeper );

public:
  typedef boost::shared_ptr<xdata::Serializable> Parameter_t;
  typedef boost::unordered_map<std::string, Parameter_t> Parameters_t;
  typedef boost::shared_ptr<Parameters_t> ParametersContext_t;
  typedef boost::unordered_map<std::string, ParametersContext_t> ParametersContextCache_t;

   // for monitoring settings
  typedef MonitoringSettingPtr_t MonitoringSetting_t;
  typedef boost::unordered_map<std::string, MonitoringSetting_t> MonitoringSettings_t;
  typedef boost::shared_ptr<MonitoringSettings_t> SettingsContext_t;
  typedef boost::unordered_map< std::string, SettingsContext_t > SettingsContextCache_t;

  typedef boost::unordered_set<std::string> Masks_t;
  typedef boost::shared_ptr<Masks_t> MasksContext_t;
  typedef boost::unordered_map<std::string, MasksContext_t> MasksContextCache_t;
  
  typedef boost::unordered_set<std::string> DisabledSet_t;

  /**
    Constructor
    @param aKey A global run-identifier
  */
  GateKeeper ( const std::string& aKey );

  /// Destructor
  virtual ~GateKeeper();


  /**
    Method to retreive configuration data from a specified path
    @param aParameterId A key used to identify the configuration data being requested of the gatekeeper
    @param aContexts A list of context identifiers (which may or may not exist) to look in for the requested parameters
    @return the requested data, or throw if the key is not found in any context
  */
  Parameter_t get ( const std::string& aSequenceId , const std::string& aCommandId , const std::string& aParameterId , const std::vector<std::string>& aContextsToLookIn ) const;
  MonitoringSetting_t getMonitoringSetting(const std::string& aState, const std::string& aMetricId, const std::vector<std::string>& aContextsToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::vector<std::string>& aContextsToLookIn) const;

  bool isEnabled(const std::string& aObjId) const;
  
  const boost::posix_time::ptime& lastUpdated();

  void setRuntimeParameter( const std::string& aParam , Parameter_t aData );

protected:
  /**
    Add a named context to the map of cached contexts
    @param aId the name of the context
    @param aContext a new xdata context, of which the Gatekeeper will take ownership
  */
  void add ( const std::string& aId , ParametersContext_t aContext );
  void add ( const std::string& aId , SettingsContext_t aContext );
  void add ( const std::string& aId , MasksContext_t aContext );

  //! Add specified ID string to the set of disabled object IDs
  void addToDisabledSet ( const std::string& aId );

private:

  /**
    Method to retreive configuration data from a specified path
    @param aParam A key used to identify the configuration data being requested of the gatekeeper
    @param aContext A context identifier (which may or may not exist) to look in for the requested parameters
    @return the requested data, or throw if the key is not found in any context
  */
  Parameter_t get ( const std::string& aParam , const std::string& aContext ) const;

  Parameter_t get ( const std::string& aNamespace , const std::string& aCommandPath , const std::string& aParameterId , const std::string& aContext ) const;

  MonitoringSetting_t getMonitoringSetting(const std::string& aStatePath, const std::string& aMetricId, const std::string& aContextsToLookIn ) const;
  MonitoringSetting_t getMonitoringSetting(const std::string& aMetricId, const std::string& aContextsToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::string& aContextsToLookIn) const;

  /// The global run-identifier
  std::string mKey;

  /// The cache of contexts
  ParametersContextCache_t mCache;
  SettingsContextCache_t mSettings;
  MasksContextCache_t mMasks;
  DisabledSet_t mDisabledObjs;

  /// The last time a context was modified
  boost::posix_time::ptime mUpdateTime;

  static const std::string kRuntimeContextLabel;
};


DEFINE_SWATCH_EXCEPTION ( ContextWithIdAlreadyExists );
DEFINE_SWATCH_EXCEPTION ( ParameterWithGivenIdAlreadyExistsInContext );
DEFINE_SWATCH_EXCEPTION ( MSettingWithGivenIdAlreadyExistsInContext );
DEFINE_SWATCH_EXCEPTION ( NonExistentKey );


} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_GATEKEEPER_HPP__ */
