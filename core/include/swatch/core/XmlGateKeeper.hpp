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

namespace pugi {
  class xml_node;
}

namespace swatch {
namespace core {

class XmlGateKeeper : public GateKeeper {
  public:

    /// Constructor
    XmlGateKeeper( Object* aToplevel , const std::string& aFileName , const std::string& aKey );

    /// Destructor
    virtual ~XmlGateKeeper();

    GateKeeper::tTable getTable( const std::string& aKey , const std::string& aId );

  private:

    std::pair< std::string , GateKeeper::tParameter > CreateParameter( pugi::xml_node& aEntry );
    std::pair< std::string , GateKeeper::tTable > CreateTable( pugi::xml_node& aTable );

    std::string mFileName;
};

DEFINE_SWATCH_EXCEPTION ( UnknownDataType );


} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_XmlGateKeeper_HPP__ */
