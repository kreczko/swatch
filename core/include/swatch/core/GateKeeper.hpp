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


namespace swatch {
namespace core {

class GateKeeper {
  public:

    /// Constructor
    GateKeeper( Object* aToplevel , const uint32_t& aKey );

    /// Destructor
    virtual ~GateKeeper();


    /**
      Method to retreive the list of data which are going to be required and preload it to check that everything is there 
    */
    bool preload();


    virtual xdata::Table* getTable( const std::string& aId ) = 0;

    /**
      Method to retreive configuration data from a specified path
      @param aPath A key used to identify the configuration data being requested of the gatekeeper
      @param aComponentID An ID specifying a table for a particular component
      @param aSystemID An ID specifying a table for a particular system
      @param aHardwareID An ID specifying a table for a particular hardwaretype
      @return the requested data
    */
     xdata::Serializable* get( const std::string& aParam , const std::vector<std::string>& aTables );

  private:
      Object* mToplevel;
      uint32_t mKey;
      typedef boost::unordered_map< std::string, xdata::Table* > tTableCache;
      tTableCache mCache;
};
 
DEFINE_SWATCH_EXCEPTION( UnknownParameter );  

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_GateKeeper_HPP__ */
