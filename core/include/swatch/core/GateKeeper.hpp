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
#include <iostream>

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/exception.hpp"

// XDAQ Headers
#include <xdata/Serializable.h>

//BOOST Headers
#include "boost/unordered_map.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace swatch
{
  namespace core
  {

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

        /**
          Constructor
          @param aToplevel The system for which we are acting as gatekeeper
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
          @param aParam A key used to identify the configuration data being requested of the gatekeeper
          @param aTables A list of table identifiers (which may or may not exist) to look in for the requested parameters
          @return the requested data, or throw if the key is not found in any table
        */
//         tParameter get ( const std::string& aParam , const std::vector<std::string>& aTables );
        tParameter get ( const std::string& aSequenceId , const std::string& aCommandId , const std::string& aParameterId , const std::vector<std::string>& aTables ) const;


        const boost::posix_time::ptime& lastUpdated();

        void SetRuntimeParameter( const std::string& aParam , tParameter aData );

      protected:
        /**
          Add a named table to the map of cached tables
          @param aId the name of the table
          @param aTable a new xdata table, of which the Gatekeeper will take ownership
        */
        void add ( const std::string& aId , tTable aTable );

      private:

        /**
          Method to retreive configuration data from a specified path
          @param aParam A key used to identify the configuration data being requested of the gatekeeper
          @param aTable A table identifier (which may or may not exist) to look in for the requested parameters
          @return the requested data, or throw if the key is not found in any table
        */
        tParameter get ( const std::string& aParam , const std::string& aTable ) const;

        tParameter get ( const std::string& aSequencePath , const std::string& aCommandPath , const std::string& aParameterId , const std::string& aTable ) const;

        /// The global run-identifier
        std::string mKey;

        /// The cache of tables
        tTableCache mCache;

        /// The last time a table was modified
        boost::posix_time::ptime mUpdateTime;

        static const std::string mRuntimeTableLabel;

    };

    DEFINE_SWATCH_EXCEPTION ( TableWithIdAlreadyExists );
    DEFINE_SWATCH_EXCEPTION ( ParameterWithGivenIdAlreadyExistsInTable );

  } /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_GateKeeper_HPP__ */
