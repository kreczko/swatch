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
  tTable lParameters(new tParameters()); //Best practice to always use named shared_ptrs
  mCache.insert(std::make_pair(kRuntimeTableLabel, lParameters));

  tSettingsTable lSettings(new tMonitoringSettings());
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

GateKeeper::tParameter GateKeeper::get(const std::string& aParam,
    const std::string& aTable) const {
  tTableCache::const_iterator lTable(mCache.find(aTable));

  if (lTable == mCache.end()) {
    return tParameter(); //perfectly acceptable for table name to not exist, just try the table with the next highest priority
  }

  tParameters::iterator lData(lTable->second->find(aParam));

  if (lData == lTable->second->end()) {
    return tParameter(); //perfectly acceptable for table name to not exist, just try the table with the next highest priority
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

GateKeeper::tParameter GateKeeper::get(const std::string& aSequencePath,
    const std::string& aCommandPath, const std::string& aParameterId,
    const std::string& aTable) const {
  tParameter lData;
  lData = get(aSequencePath, aTable);
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

  return tParameter();
}

GateKeeper::tParameter GateKeeper::get(const std::string& aSequenceId,
    const std::string& aCommandId, const std::string& aParameterId,
    const std::vector<std::string>& aTablesToLookIn) const {

  std::string lCommandPath(aCommandId + "." + aParameterId);
  std::string lSequencePath(aSequenceId + "." + lCommandPath);

  //See if the value was set at Run-time
  tParameter lData;
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

  return tParameter();
}

GateKeeper::tMonitoringSetting GateKeeper::getMonitoringSetting(
    const std::string& aState, const std::string& aMetricId,
    const std::vector<std::string>& aTablesToLookIn) const {
  std::string statePath(aState + "." + aMetricId);

  //See if the value was set at Run-time
  tMonitoringSetting setting;
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

  return tMonitoringSetting();
}

GateKeeper::tMonitoringSetting GateKeeper::getMonitoringSetting(
    const std::string& aStatePath, const std::string& aMetricId,
    const std::string& aTableToLookIn) const {
  tMonitoringSetting setting;
  setting = getMonitoringSetting(aStatePath, aTableToLookIn);
  if (setting) {
    return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  setting = getMonitoringSetting(aMetricId, aTableToLookIn);
  if (setting) {
    return setting; //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
  }

  return tMonitoringSetting();
}

GateKeeper::tMonitoringSetting GateKeeper::getMonitoringSetting(
    const std::string& aMetricId, const std::string& aTableToLookIn) const {

  tSettingsTableCache::const_iterator lTable(mSettings.find(aTableToLookIn));

  if (lTable == mSettings.end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return tMonitoringSetting();
  }

  tMonitoringSettings::iterator settings(lTable->second->find(aMetricId));

  if (settings == lTable->second->end()) {
    //perfectly acceptable for table name to not exist, just try the table with the next highest priority
    return tMonitoringSetting();
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


void GateKeeper::add(const std::string& aId, tTable aTable) {
  tTableCache::iterator lTableIt(mCache.find(aId));

  if (lTableIt != mCache.end()) {
    throw TableWithIdAlreadyExists(
        "Table With Id '" + aId + "' already exists");
  }

  mCache.insert(std::make_pair(aId, aTable));
  mUpdateTime = boost::posix_time::microsec_clock::universal_time();
}

void GateKeeper::add(const std::string& aId, tSettingsTable aTable) {
  tSettingsTableCache::iterator lTableIt(mSettings.find(aId));
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


const boost::posix_time::ptime& GateKeeper::lastUpdated() {
  return mUpdateTime;
}

void GateKeeper::setRuntimeParameter(const std::string& aParam,
    tParameter aData) {
  tTableCache::iterator lTable(mCache.find(kRuntimeTableLabel));
  tParameters::iterator lIt(lTable->second->find(aParam));

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
  for (GateKeeper::tTableCache::const_iterator lTableIt(
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

  for (GateKeeper::tSettingsTableCache::const_iterator lTableIt(aGateKeeper.mSettings.begin());
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
