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
  typedef boost::shared_ptr<Parameters_t> ParametersTable_t;
  typedef boost::unordered_map<std::string, ParametersTable_t> ParametersTableCache_t;

   // for monitoring settings
  typedef MonitoringSettingPtr_t MonitoringSetting_t;
  typedef boost::unordered_map<std::string, MonitoringSetting_t> MonitoringSettings_t;
  typedef boost::shared_ptr<MonitoringSettings_t> SettingsTable_t;
  typedef boost::unordered_map< std::string, SettingsTable_t > SettingsTableCache_t;

  typedef boost::unordered_set<std::string> Masks_t;
  typedef boost::shared_ptr<Masks_t> MasksTable_t;
  typedef boost::unordered_map<std::string, MasksTable_t> MasksTableCache_t;
  
  typedef boost::unordered_set<std::string> DisabledSet_t;

  /**
    Constructor
    @param aKey A global run-identifier
  */
  GateKeeper ( const std::string& aKey );

  /// Destructor
  virtual ~GateKeeper();


//        /**
//          Method to retreive the list of data which are going to be required and preload it to check that everything is there
//          @return whether we were successful or not
//        */
//        bool preload();

//        /**
//          Pure virtual method which derived classes must implement to create an xdata table from a given ID
//          @param aId the ID of the table to create
//          @return the new xdata table
//        */
//        virtual tTable getTable ( const std::string& aKey , const std::string& aId ) = 0;

  /**
    Method to retreive configuration data from a specified path
    @param aParameterId A key used to identify the configuration data being requested of the gatekeeper
    @param aTables A list of table identifiers (which may or may not exist) to look in for the requested parameters
    @return the requested data, or throw if the key is not found in any table
  */
  Parameter_t get ( const std::string& aSequenceId , const std::string& aCommandId , const std::string& aParameterId , const std::vector<std::string>& aTablesToLookIn ) const;
  MonitoringSetting_t getMonitoringSetting(const std::string& aState, const std::string& aMetricId, const std::vector<std::string>& aTablesToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::vector<std::string>& aTablesToLookIn) const;

  bool isEnabled(const std::string& aObjId) const;
  
  const boost::posix_time::ptime& lastUpdated();

  void setRuntimeParameter( const std::string& aParam , Parameter_t aData );

protected:
  /**
    Add a named table to the map of cached tables
    @param aId the name of the table
    @param aTable a new xdata table, of which the Gatekeeper will take ownership
  */
  void add ( const std::string& aId , ParametersTable_t aTable );
  void add ( const std::string& aId , SettingsTable_t aTable );
  void add ( const std::string& aId , MasksTable_t aTable );

  //! Add specified ID string to the set of disabled object IDs
  void addToDisabledSet ( const std::string& aId );

private:

  /**
    Method to retreive configuration data from a specified path
    @param aParam A key used to identify the configuration data being requested of the gatekeeper
    @param aTable A table identifier (which may or may not exist) to look in for the requested parameters
    @return the requested data, or throw if the key is not found in any table
  */
  Parameter_t get ( const std::string& aParam , const std::string& aTable ) const;

  Parameter_t get ( const std::string& aNamespace , const std::string& aCommandPath , const std::string& aParameterId , const std::string& aTable ) const;

  MonitoringSetting_t getMonitoringSetting(const std::string& aStatePath, const std::string& aMetricId, const std::string& aTableToLookIn ) const;
  MonitoringSetting_t getMonitoringSetting(const std::string& aMetricId, const std::string& aTableToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::string& aTablesToLookIn) const;

  /// The global run-identifier
  std::string mKey;

  /// The cache of tables
  ParametersTableCache_t mCache;
  SettingsTableCache_t mSettings;
  MasksTableCache_t mMasks;
  DisabledSet_t mDisabledObjs;

  /// The last time a table was modified
  boost::posix_time::ptime mUpdateTime;

  static const std::string kRuntimeTableLabel;
};


DEFINE_SWATCH_EXCEPTION ( TableWithIdAlreadyExists );
DEFINE_SWATCH_EXCEPTION ( ParameterWithGivenIdAlreadyExistsInTable );
DEFINE_SWATCH_EXCEPTION ( MSettingWithGivenIdAlreadyExistsInTable );


} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_GATEKEEPER_HPP__ */
