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
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace core {

class GateKeeper {
  public:

    /// Constructor
    GateKeeper();

    /// Destructor
    virtual ~GateKeeper();


    /**
      Method to retreive the list of data which are going to be required and preload it to check that everything is there 
    */
    bool preloadAndCheck();


    /**
      Method to retreive configuration data from a specified path
      @param aPath A key used to identify the configuration data being requested of the gatekeeper
      @param aComponentID An ID specifying a table for a particular component
      @param aSystemID An ID specifying a table for a particular system
      @param aHardwareID An ID specifying a table for a particular hardwaretype
      @return the requested data
    */
    xdata::Serializable& get( const std::string& aPath , const std::string* aComponentID = NULL , const std::string* aSystemID = NULL , const std::string* aHardwareID = NULL );

  private:

};

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_GateKeeper_HPP__ */
