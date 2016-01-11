#include "swatch/core/GateKeeper.hpp"

// Boost Headers
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"

namespace swatch {
namespace core {

GateKeeper::GateKeeper(const std::string& aKey) :
        mKey(aKey),
        mCache(),
        mSettings(),
        mUpdateTime() {
  ParametersTable_t lParameters(new Parameters_t()); //Best practice to always use named shared_ptrs
  mCache.insert(std::make_pair(kRuntimeTableLabel, lParameters));

  SettingsTable_t lSettings(new MonitoringSettings_t());
  mSettings.insert(std::make_pair(kRuntimeTableLabel, lSettings));
}

GateKeeper::~GateKeeper() {
}

//    bool GateKeeper::preload()
//    {
//      // Iterate over all objects under the top level: if they are CommandSequences, claim them and cache the resources they will use
//      for ( Object::iterator lIt ( mToplevel->begin() ); lIt != mToplevel->end(); ++ lIt )
//      {
//        CommandSequence* lCommandSequence ( dynamic_cast< CommandSequence* > ( & ( *lIt ) ) );
//
//        if ( lCommandSequence )
//        {
//          const std::vector<std::string>& lTables = lCommandSequence->getTables();
//
//          for ( std::vector<std::string>::const_iterator lIt2 ( lTables.begin() ) ; lIt2!=lTables.end() ; ++lIt2 )
//          {
//            tTableCache::iterator lTableIt ( mCache.find ( *lIt2 ) );
//
//            if ( lTableIt != mCache.end() )
//            {
//              continue;
//            }
//
//            tTable lTable ( getTable ( mKey , *lIt2 ) ); //perfectly acceptable to return NULL - no such table exists
//
//            if ( lTable )
//            {
//              mCache.insert ( std::make_pair ( *lIt2 , lTable ) );  //Could use add method here for clarity, but there is no point rechecking the existence of the Id in the cache
//              mUpdateTime = boost::posix_time::microsec_clock::universal_time();
//            }
//          }
//
//
//          for( CommandSequence::tCommandVector::const_iterator lIt2( lCommandSequence->getCommands().begin()) ; lIt2 != lCommandSequence->getCommands().end() ; ++lIt2 )
//          {
//            Command& lCommand = **lIt2; //Utility function
//            std::set< std::string > lKeys( lCommand.getDefaultParams().keys() );
//            for( std::set< std::string >::iterator lIt3( lKeys.begin() ); lIt3!=lKeys.end(); ++lIt3 )
//            {
//              get ( lCommandSequence->getId() , lCommand.getId() , *lIt3 , lTables );
//            }
//          }
//
////          std::set<std::string> lParams = lCommandSequence->getParams();
////          for ( std::set<std::string>::const_iterator lIt2 ( lParams.begin() ) ; lIt2!=lParams.end() ; ++lIt2 )
////          {
////             get ( *lIt2 , lTables );
////          }
//        }
//      }
//
//      return true;
//    }

GateKeeper::Parameter_t GateKeeper::get(const std::string& aParam,
    const std::string& aTable) const {
  ParametersTableCache_t::const_iterator lTable(mCache.find(aTable));

  if (lTable == mCache.end()) {
    return Parameter_t(); //perfectly acceptable for table name to not exist, just try the table with the next highest priority
  }

  Parameters_t::iterator lData(lTable->second->find(aParam));

  if (lData == lTable->second->end()) {
    return Parameter_t(); //perfectly acceptable for table name to not exist, just try the table with the next highest priority
  }

  return lData->second;  //We found data!
}

//     GateKeeper::tParameter GateKeeper::get ( const std::string& aParam , const std::vector<std::string>& aTables )
//     {
//       //See if the value was set at Run-time
//       tParameter lData ( get( aParam , mRuntimeTableLabel ) );
//       if ( lData )
//       {
//         return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
//       }       
// 
//       //We could add runtime overriding of values to the GateKeeper and check them first...
//       for ( std::vector<std::string>::const_iterator lIt ( aTables.begin() ) ; lIt!=aTables.end() ; ++lIt )
//       {
//         tParameter lData ( get( aParam , *lIt ) );
//         if ( lData )
//         {
//           return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
//         }    
//       }
// 
// //       throw UnknownParameter ( "Unable to find parameter '"+aParam+"' in any listed table" ); //no table contains the requested parameter - problem!
//       return tParameter(); //to stop the compiler complaining...
//     }

GateKeeper::Parameter_t GateKeeper::get(const std::string& aNamespace,
    const std::string& aCommandPath, const std::string& aParameterId,
    const std::string& aTable) const {
  Parameter_t lData;
  lData = get(aNamespace, aTable);
  if (lData) {
    return lData; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  lData = get(aCommandPath, aTable);
  if (lData) {
    return lData; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  lData = get(aParameterId, aTable);
  if (lData) {
    return lData; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  return Parameter_t();
}

GateKeeper::Parameter_t GateKeeper::get(const std::string& aSequenceId,
    const std::string& aCommandId, const std::string& aParameterId,
    const std::vector<std::string>& aTablesToLookIn) const {

  std::string lCommandPath(aCommandId + "." + aParameterId);
  std::string lSequencePath(aSequenceId + "." + lCommandPath);

  //See if the value was set at Run-time
  Parameter_t lData;
  lData = get(lSequencePath, lCommandPath, aParameterId, kRuntimeTableLabel);
  if (lData) {
    return lData; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  //We could add runtime overriding of values to the GateKeeper and check them first...
  for (std::vector<std::string>::const_iterator lIt(aTablesToLookIn.begin());
      lIt != aTablesToLookIn.end(); ++lIt) {
    LOG(logger::kDebug) << "Searching : " << lSequencePath << ", "
        << lCommandPath << ", " << aParameterId << " in  " << *lIt << std::endl;
    lData = get(lSequencePath, lCommandPath, aParameterId, *lIt);
    if (lData) {
      return lData; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
    }
  }

  return Parameter_t();
}

GateKeeper::MonitoringSetting_t GateKeeper::getMonitoringSetting(
    const std::string& aState, const std::string& aMetricId,
    const std::vector<std::string>& aTablesToLookIn) const {
  std::string statePath(aState + "." + aMetricId);

  //See if the value was set at Run-time
  MonitoringSetting_t setting;
  setting = getMonitoringSetting(statePath, aMetricId, kRuntimeTableLabel);
  if (setting) {
    return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  //We could add runtime overriding of values to the GateKeeper and check them first...
  for (std::vector<std::string>::const_iterator lIt(aTablesToLookIn.begin());
      lIt != aTablesToLookIn.end(); ++lIt) {
    LOG(logger::kDebug) << "Searching : " << statePath << ", " << aMetricId
        << " in  " << *lIt << std::endl;
    setting = getMonitoringSetting(statePath, aMetricId, *lIt);
    if (setting) {
      return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
    }
  }

  return MonitoringSetting_t();
}

GateKeeper::MonitoringSetting_t GateKeeper::getMonitoringSetting(
    const std::string& aStatePath, const std::string& aMetricId,
    const std::string& aTableToLookIn) const {
  MonitoringSetting_t setting;
  setting = getMonitoringSetting(aStatePath, aTableToLookIn);
  if (setting) {
    return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  setting = getMonitoringSetting(aMetricId, aTableToLookIn);
  if (setting) {
    return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  return MonitoringSetting_t();
}

GateKeeper::MonitoringSetting_t GateKeeper::getMonitoringSetting(
    const std::string& aMetricId, const std::string& aTableToLookIn) const {

  SettingsTableCache_t::const_iterator lTable(mSettings.find(aTableToLookIn));

  if (lTable == mSettings.end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return MonitoringSetting_t();
  }

  MonitoringSettings_t::iterator settings(lTable->second->find(aMetricId));

  if (settings == lTable->second->end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return MonitoringSetting_t();
  }

  return settings->second;  //We found data!
}


bool GateKeeper::getMask(const std::string& aObjId, const std::vector<std::string>& aTablesToLookIn) const
{
  bool lMask = false;
  
  for (std::vector<std::string>::const_iterator lIt(aTablesToLookIn.begin()); lIt != aTablesToLookIn.end(); ++lIt) {
    LOG(logger::kDebug) << "Searching for mask for '" << aObjId << "', in table " << *lIt << std::endl;
    lMask = getMask(aObjId, *lIt);
    if (lMask)
      return lMask;
  }
  
  return false;
}


bool GateKeeper::getMask(const std::string& aObjId, const std::string& aTableToLookIn) const
{
   MasksTableCache_t::const_iterator lTable(mMasks.find(aTableToLookIn));

  if (lTable == mMasks.end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return false;
  }

  Masks_t::const_iterator lMaskIt = lTable->second->find(aObjId);

  if (lMaskIt == lTable->second->end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return false;
  }

  return true;  //We found data!
}


bool GateKeeper::isEnabled(const std::string& aObjId) const
{
  return (mDisabledObjs.find(aObjId) == mDisabledObjs.end());
}


void GateKeeper::add(const std::string& aId, ParametersTable_t aTable) {
  ParametersTableCache_t::iterator lTableIt(mCache.find(aId));

  if (lTableIt != mCache.end()) {
    throw TableWithIdAlreadyExists(
        "Table With Id '" + aId + "' already exists");
  }

  mCache.insert(std::make_pair(aId, aTable));
  mUpdateTime = boost::posix_time::microsec_clock::universal_time();
}

void GateKeeper::add(const std::string& aId, SettingsTable_t aTable) {
  SettingsTableCache_t::iterator lTableIt(mSettings.find(aId));
  if (lTableIt != mSettings.end()) {
    throw TableWithIdAlreadyExists(
        "Table of monitoring settings With Id '" + aId + "' already exists");
  }

  mSettings.insert(std::make_pair(aId, aTable));
}


void GateKeeper::add(const std::string& aId, MasksTable_t aTable)
{
  MasksTableCache_t::const_iterator lTableIt(mMasks.find(aId));
  if (lTableIt != mMasks.end())
    throw TableWithIdAlreadyExists("Table of masks with Id '" + aId + "' already exists");

  mMasks.insert(std::make_pair(aId, aTable));
}


void GateKeeper::addToDisabledSet ( const std::string& aId )
{
  DisabledSet_t::const_iterator lIt(mDisabledObjs.find(aId));
  if (lIt != mDisabledObjs.end())
    throw TableWithIdAlreadyExists("ID path '"+ aId + "' is already present in set of disabled IDs");
  
  mDisabledObjs.insert(aId);
}


const boost::posix_time::ptime& GateKeeper::lastUpdated() {
  return mUpdateTime;
}

void GateKeeper::setRuntimeParameter(const std::string& aParam,
    Parameter_t aData) {
  ParametersTableCache_t::iterator lTable(mCache.find(kRuntimeTableLabel));
  Parameters_t::iterator lIt(lTable->second->find(aParam));

  if (lIt != lTable->second->end()) {
    lIt->second = aData;
    return;
  }
  lTable->second->insert(std::make_pair(aParam, aData));
}

const std::string GateKeeper::kRuntimeTableLabel = std::string("__runtime__");

std::ostream& operator<<(std::ostream& aStr,
    const swatch::core::GateKeeper& aGateKeeper) {
  std::string lDelimeter(100, '-');
  for (GateKeeper::ParametersTableCache_t::const_iterator lTableIt(
      aGateKeeper.mCache.begin()); lTableIt != aGateKeeper.mCache.end();
      ++lTableIt) {
    aStr << lDelimeter << std::endl;
    aStr << "TABLE : " << lTableIt->first << std::endl;
    aStr << lDelimeter << std::endl;

    std::set<std::string> names;
    boost::copy(*(lTableIt->second) | boost::adaptors::map_keys,
        std::inserter(names, names.begin()));

    BOOST_FOREACH( const std::string& name, names ) {
      aStr << " " << name << " : " << lTableIt->second->at(name)->toString()
          << std::endl;
    }

    aStr << lDelimeter << std::endl;
  }

  for (GateKeeper::SettingsTableCache_t::const_iterator lTableIt(aGateKeeper.mSettings.begin());
      lTableIt != aGateKeeper.mSettings.end(); ++lTableIt) {
      aStr << lDelimeter << std::endl;
      aStr << "TABLE : " << lTableIt->first << std::endl;
      aStr << lDelimeter << std::endl;

      std::set<std::string> names;
      boost::copy(*(lTableIt->second) | boost::adaptors::map_keys,
          std::inserter(names, names.begin()));

      BOOST_FOREACH( const std::string& name, names ) {
        monitoring::Status lMonStatus(lTableIt->second->at(name)->getStatus());
        aStr << " " << name << " : " << lMonStatus << std::endl;
      }

      aStr << lDelimeter << std::endl;
    }

  return aStr;
}

} /* namespace core */
} /* namespace swatch */
