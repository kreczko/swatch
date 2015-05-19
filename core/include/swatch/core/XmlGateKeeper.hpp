/**
 * @file    XmlGateKeeper.hpp
 * @author  arose
 * @brief   Guardian to the underworld that is the online database
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_XmlGateKeeper_HPP__
#define __SWATCH_CORE_XmlGateKeeper_HPP__

// C++ Headers
#include <string>

// Swatch Headers
#include "swatch/core/GateKeeper.hpp"


namespace swatch {
namespace core {

class XmlGateKeeper : public GateKeeper {
  public:

    /// Constructor
    XmlGateKeeper( Object* aToplevel , const std::string& aFileName , const uint32_t& aKey );

    /// Destructor
    virtual ~XmlGateKeeper();

    xdata::Table* getTable( const std::string& aId );

  private:
};

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_XmlGateKeeper_HPP__ */
