/**
 * @file    Utilities.hpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

// boost headers
#include "boost/property_tree/json_parser.hpp"

// SWATCH headers
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/system/SystemStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/system/LinkStub.hpp"


namespace swatch {
namespace dtm {

DaqTTCStub treeToDaqTTCStub( const boost::property_tree::ptree& t );

} // namespace dtm
} // namespace swatch


namespace swatch {
namespace system {

SystemStub treeToSystemStub( const boost::property_tree::ptree& t );

CrateStub treeToCrateStub( const boost::property_tree::ptree& t );

void treeToLinkStub(const boost::property_tree::ptree& aPTree, std::vector<LinkStub>& aLinkStubs);

//void pushBackLinkStubs(std::vector<LinkStub>& aLinkStubs, const std::string& aName, const std::string& aSrc, const std::string& aDst);


} // namespace system
} // namespace swatch
