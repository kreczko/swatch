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
  typedef boost::shared_ptr<xdata::Serializable> tParameter;
  typedef boost::unordered_map< std::string, tParameter > tParameters;
  typedef boost::shared_ptr<tParameters> tTable;
  typedef boost::unordered_map< std::string, tTable > tTableCache;

   // for monitoring settings
  typedef tMonitoringSettingPtr tMonitoringSetting;
  typedef boost::unordered_map<std::string, tMonitoringSetting> tMonitoringSettings;
  typedef boost::shared_ptr<tMonitoringSettings> tSettingsTable;
  typedef boost::unordered_map< std::string, tSettingsTable > tSettingsTableCache;

  typedef boost::unordered_set<std::string> Masks_t;
  typedef boost::shared_ptr<Masks_t> MasksTable_t;
  typedef boost::unordered_map<std::string, MasksTable_t> MasksTableCache_t;

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
  tParameter get ( const std::string& aSequenceId , const std::string& aCommandId , const std::string& aParameterId , const std::vector<std::string>& aTablesToLookIn ) const;
  tMonitoringSetting getMonitoringSetting(const std::string& aState, const std::string& aMetricId, const std::vector<std::string>& aTablesToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::vector<std::string>& aTablesToLookIn) const;

  const boost::posix_time::ptime& lastUpdated();

  void SetRuntimeParameter( const std::string& aParam , tParameter aData );

protected:
  /**
    Add a named table to the map of cached tables
    @param aId the name of the table
    @param aTable a new xdata table, of which the Gatekeeper will take ownership
  */
  void add ( const std::string& aId , tTable aTable );
  void add ( const std::string& aId , tSettingsTable aTable );
  void add ( const std::string& aId , MasksTable_t aTable );

private:

  /**
    Method to retreive configuration data from a specified path
    @param aParam A key used to identify the configuration data being requested of the gatekeeper
    @param aTable A table identifier (which may or may not exist) to look in for the requested parameters
    @return the requested data, or throw if the key is not found in any table
  */
  tParameter get ( const std::string& aParam , const std::string& aTable ) const;

  tParameter get ( const std::string& aSequencePath , const std::string& aCommandPath , const std::string& aParameterId , const std::string& aTable ) const;

  tMonitoringSetting getMonitoringSetting(const std::string& aStatePath, const std::string& aMetricId, const std::string& aTableToLookIn ) const;
  tMonitoringSetting getMonitoringSetting(const std::string& aMetricId, const std::string& aTableToLookIn ) const;
  bool getMask(const std::string& aObjId, const std::string& aTablesToLookIn) const;

  /// The global run-identifier
  std::string mKey;

  /// The cache of tables
  tTableCache mCache;
  tSettingsTableCache mSettings;
  MasksTableCache_t mMasks;

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
