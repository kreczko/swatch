// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

// C++ Headers
#include <iomanip>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/Link.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
//#include "swatch/system/SystemFactory.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/system/Utilities.hpp"

// XDAQ Headers
#include "xdata/String.h"

// Namespace resolution
using namespace std;

namespace swco  = swatch::core;
namespace swpro = swatch::processor;
namespace swsys = swatch::system;

using swatch::processor::test::DummyProcessor;
using swatch::system::test::DummyAMC13Manager;

int main(int argc, char const *argv[])
{
using namespace boost::assign;
    
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::XParameterSet;
    ptree pt;

    read_json(swco::shellExpandPath("${SWATCH_ROOT}/test/etc/swatch/test/testdb.json"), pt);
    swsys::SystemStub sysset = swatch::system::treeToSystemPars(pt);

    swatch::system::System* mysys = swco::Factory::get()->make<swsys::System>("swatch::system::SystemCreator", sysset);
    cout << "Here's a brand new system: " << mysys << endl; 
    
    return 0;
}
