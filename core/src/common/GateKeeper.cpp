
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/CommandSequence.hpp"


namespace swatch
{
  namespace core
  {

    GateKeeper::GateKeeper ( Object* aToplevel , const uint32_t& aKey ) :
      mToplevel ( aToplevel ),
      mKey ( aKey ),
      mCache()
    {}

    GateKeeper::~GateKeeper()
    {
      for ( boost::unordered_map< std::string, xdata::Table* >::iterator lIt ( mCache.begin() ); lIt!=mCache.end(); ++lIt )
      {
        delete lIt->second;
        lIt->second = NULL;
      }

      mCache.clear();
    }

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

            xdata::Table* lTable ( getTable ( *lIt2 ) ); //perfectly acceptable to return NULL - no such table exists

            if ( lTable )
            {
              mCache.insert ( std::make_pair ( *lIt2 , lTable ) );  //Could use add method here, but there is no point rechecking the existence of the Id in the cache
            }
          }

          std::set<std::string> lParams = lCommandSequence->getParams();

          for ( std::set<std::string>::const_iterator lIt2 ( lParams.begin() ) ; lIt2!=lParams.end() ; ++lIt2 )
          {
            get ( *lIt2 , lTables );
          }
        }
      }

      return true;
    }


    xdata::Serializable* GateKeeper::get ( const std::string& aParam , const std::vector<std::string>& aTables )
    {
      //We could add runtime overriding of values to the GateKeeper and check them first...
      for ( std::vector<std::string>::const_iterator lIt ( aTables.begin() ) ; lIt!=aTables.end() ; ++lIt )
      {
        tTableCache::iterator lTable ( mCache.find ( *lIt ) );

        if ( lTable == mCache.end() )
        {
          continue;  //perfectly acceptable for table name to not exist, just try the table with the next highest priority
        }

        try
        {
          xdata::Serializable* lData ( lTable->second->getValueAt ( mKey , aParam ) );

          if ( lData )
          {
            return lData;  //perfectly acceptable for specific table not hold the requested data, just try the table with the next highest priority
          }
        }
        catch ( const xdata::exception::Exception& ) {}
      }

      throw UnknownParameter ( "Unable to find parameter '"+aParam+"' in any listed table" ); //no table contains the requested parameter - problem!
      return NULL; //to stop the compiler complaining...
    }


    void GateKeeper::add ( const std::string& aId , xdata::Table* aTable )
    {
      tTableCache::iterator lTableIt ( mCache.find ( aId ) );

      if ( lTableIt != mCache.end() )
      {
        throw TableWithIdAlreadyExists ( "Table With Id '"+aId+"' Already Exists" );
      }

      mCache.insert ( std::make_pair ( aId , aTable ) );
    }


  } /* namespace core */
} /* namespace swatch */
