/**
 * @file    Utilities.hpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"

// Boost Headers
#include <boost/property_tree/json_parser.hpp>


namespace swatch {
namespace system {

swatch::core::ParameterSet treeToAMC13Pars( const boost::property_tree::ptree& t );
swatch::core::ParameterSet treeToSystemPars( const boost::property_tree::ptree& t );
swatch::core::ParameterSet treeToCratePars( const boost::property_tree::ptree& t );

} // namespace processor
} // namespace swatch
