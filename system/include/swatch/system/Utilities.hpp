/**
 * @file    Utilities.hpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

// boost headers
#include "boost/property_tree/json_parser.hpp"

// SWATCH headers
#include "swatch/system/DaqTTCStub.hpp"
#include "swatch/system/SystemStub.hpp"
#include "swatch/system/CrateStub.hpp"


namespace swatch {
namespace system {

swatch::system::DaqTTCStub treeToDaqTTCStub( const boost::property_tree::ptree& t );

swatch::system::SystemStub treeToSystemPars( const boost::property_tree::ptree& t );

swatch::system::CrateStub treeToCrateStub( const boost::property_tree::ptree& t );

} // namespace processor
} // namespace swatch
