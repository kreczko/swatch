/**
 * @file    GateKeeper.hpp
 * @author  arose
 * @brief   Guardian to the underworld that is the online database
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_GateKeeper_HPP__
#define __SWATCH_CORE_GateKeeper_HPP__

// C++ Headers
#include <string>

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/exception.hpp"

// XDAQ Headers
#include <xdata/Table.h>
#include <xdata/Serializable.h>

//BOOST Headers
#include "boost/unordered_map.hpp"


namespace swatch
{
  namespace core
  {

    class GateKeeper
    {
      public:

        /**
          Constructor
          @param aToplevel The system for which we are acting as gatekeeper
          @param aKey A global run-identifier
        */
        GateKeeper ( Object* aToplevel , const uint32_t& aKey );

        /// Destructor
        virtual ~GateKeeper();


        /**
          Method to retreive the list of data which are going to be required and preload it to check that everything is there
          @return whether we were successful or not
        */
        bool preload();

        /**
          Pure virtual method which derived classes must implement to create an xdata table from a given ID
          @param aId the ID of the table to create
          @return the new xdata table
        */
        virtual xdata::Table* getTable ( const std::string& aId ) = 0;

        /**
          Method to retreive configuration data from a specified path
          @param aParam A key used to identify the configuration data being requested of the gatekeeper
          @param aTables A list of table identifiers (which may or may not exist) to look in for the requested parameters
          @return the requested data, or throw if the key is not found in any table
        */
        xdata::Serializable* get ( const std::string& aParam , const std::vector<std::string>& aTables );

      protected:
        /**
          Add a named table to the map of cached tables
          @param aId the name of the table
          @param aTable a new xdata table, of which the Gatekeeper will take ownership
        */
        void add ( const std::string& aId , xdata::Table* aTable );

      private:
        /// The system for which we are acting as gatekeeper
        Object* mToplevel;

        /// The global run-identifier
        uint32_t mKey;

        typedef boost::unordered_map< std::string, xdata::Table* > tTableCache;
        /// The cache of tables
        tTableCache mCache;
    };

    DEFINE_SWATCH_EXCEPTION ( TableWithIdAlreadyExists );
    DEFINE_SWATCH_EXCEPTION ( UnknownParameter );

  } /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_GateKeeper_HPP__ */
