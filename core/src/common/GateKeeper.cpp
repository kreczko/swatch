
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace swatch
{
  namespace core
  {

    GateKeeper::GateKeeper ( Object* aToplevel , const std::string& aKey ) :
      mToplevel ( aToplevel ),
      mKey ( aKey ),
      mCache(),
      mUpdateTime()
    {
      tTable lParameters( new tParameters() ); //Best practice to always use named shared_ptrs
      mCache.insert( std::make_pair( mRuntimeTableLabel , lParameters ) );
    }

    GateKeeper::~GateKeeper()
    {}

    bool GateKeeper::preload()
    {
      // Iterate over all objects under the top level: if they are CommandSequences, claim them and cache the resources they will use
      for ( Object::iterator lIt ( mToplevel->begin() ); lIt != mToplevel->end(); ++ lIt )
      {
        CommandSequence* lCommandSequence ( dynamic_cast< CommandSequence* > ( & ( *lIt ) ) );

        if ( lCommandSequence )
        {
          lCommandSequence->setGateKeeper ( this );
          const std::vector<std::string>& lTables = lCommandSequence->getTables();

          for ( std::vector<std::string>::const_iterator lIt2 ( lTables.begin() ) ; lIt2!=lTables.end() ; ++lIt2 )
          {
            tTableCache::iterator lTableIt ( mCache.find ( *lIt2 ) );

            if ( lTableIt != mCache.end() )
            {
              continue;
            }

            tTable lTable ( getTable ( mKey , *lIt2 ) ); //perfectly acceptable to return NULL - no such table exists

            if ( lTable )
            {
              mCache.insert ( std::make_pair ( *lIt2 , lTable ) );  //Could use add method here for clarity, but there is no point rechecking the existence of the Id in the cache
              mUpdateTime = boost::posix_time::microsec_clock::universal_time();
            }
          }


          for( CommandSequence::tCommandVector::iterator lIt2( lCommandSequence->mCommands.begin()) ; lIt2 != lCommandSequence->mCommands.end() ; ++lIt2 )
          {
            Command& lCommand( lCommandSequence->getCommand( *lIt2 ) ); //Utility function
            std::set< std::string > lKeys( lCommand.getDefaultParams().keys() );
            for( std::set< std::string >::iterator lIt3( lKeys.begin() ); lIt3!=lKeys.end(); ++lIt3 )
            {
              get ( lCommandSequence->id() , lCommand.id() , *lIt3 , lTables );
            }
          }

//          std::set<std::string> lParams = lCommandSequence->getParams();
//          for ( std::set<std::string>::const_iterator lIt2 ( lParams.begin() ) ; lIt2!=lParams.end() ; ++lIt2 )
//          {
//             get ( *lIt2 , lTables );
//          }
        }
      }

      return true;
    }


    GateKeeper::tParameter GateKeeper::get ( const std::string& aParam , const std::string& aTable )
    {
      tTableCache::iterator lTable ( mCache.find ( aTable ) );

      if ( lTable == mCache.end() )
      {
        return tParameter();  //perfectly acceptable for table name to not exist, just try the table with the next highest priority
      }

      tParameters::iterator lData ( lTable->second->find( aParam ) );

      if ( lData == lTable->second->end() )
      {
        return tParameter();  //perfectly acceptable for table name to not exist, just try the table with the next highest priority
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


    GateKeeper::tParameter GateKeeper::get ( const std::string& aSequencePath , const std::string& aCommandPath , const std::string& aTable )
    {
      tParameter lData;
      lData = get( aSequencePath , aTable );
      if ( lData )
      {
        return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
      }       

      lData = get( aCommandPath , aTable );
      if ( lData )
      {
        return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
      }   

      return tParameter(); 
    }


    GateKeeper::tParameter GateKeeper::get ( const std::string& aSequenceId , const std::string& aCommandId , const std::string& aParameterId , const std::vector<std::string>& aTables )
    {

      std::string lCommandPath( aCommandId + "." + aParameterId );
      std::string lSequencePath( aSequenceId + "." + lCommandPath );

      //See if the value was set at Run-time
      tParameter lData;
      lData = get( lSequencePath , lCommandPath , mRuntimeTableLabel );
      if ( lData )
      {
        return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
      }       

      //We could add runtime overriding of values to the GateKeeper and check them first...
      for ( std::vector<std::string>::const_iterator lIt ( aTables.begin() ) ; lIt!=aTables.end() ; ++lIt )
      {
        lData = get(  lSequencePath , lCommandPath , *lIt );
        if ( lData )
        {
          return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
        }
      }

      return tParameter(); 
    }



    void GateKeeper::add ( const std::string& aId , tTable aTable )
    {
      tTableCache::iterator lTableIt ( mCache.find ( aId ) );

      if ( lTableIt != mCache.end() )
      {
        throw TableWithIdAlreadyExists ( "Table With Id '"+aId+"' Already Exists" );
      }

      mCache.insert ( std::make_pair ( aId , aTable ) );
      mUpdateTime = boost::posix_time::microsec_clock::universal_time();
    }


    const boost::posix_time::ptime& GateKeeper::lastUpdated()
    {
      return mUpdateTime;
    }


    void GateKeeper::SetRuntimeParameter( const std::string& aParam , tParameter aData )
    {
      tTableCache::iterator lTable ( mCache.find ( mRuntimeTableLabel ) );
      tParameters::iterator lIt ( lTable->second->find ( aParam ) );

      if ( lIt != lTable->second->end() )
      {
        lIt->second = aData;
        return;
      }
      lTable->second->insert( std::make_pair( aParam , aData ) );
    }

    const std::string GateKeeper::mRuntimeTableLabel = std::string("__runtime__");



    std::ostream& operator<< ( std::ostream& aStr , const swatch::core::GateKeeper& aGateKeeper )
    {
      std::string lDelimeter( 100, '-' );
      for( GateKeeper::tTableCache::const_iterator lTableIt ( aGateKeeper.mCache.begin() ); lTableIt != aGateKeeper.mCache.end() ; ++ lTableIt )
      {
        aStr << lDelimeter << std::endl;
        aStr << "TABLE : " << lTableIt->first << std::endl;
        aStr << lDelimeter << std::endl;
        for( GateKeeper::tParameters::const_iterator lIt ( lTableIt->second->begin() ); lIt != lTableIt->second->end() ; ++lIt )
        {
          aStr << "  " << lIt->first << " : " << lIt->second->toString() << std::endl;          
        }
        aStr << lDelimeter << std::endl;
      }
      return aStr;
    }

  } /* namespace core */
} /* namespace swatch */
