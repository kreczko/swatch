/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/Factory.hpp"

#include "swatch/system/Utilities.hpp"
#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// uHAL Headers
#include "uhal/log/log.hpp"

// Boost Headers
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;
using swatch::core::XParameterSet;
using swatch::core::shellExpandPath;

int main(int argc, char** argv) {

    uhal::setLogLevelTo( uhal::Warning() );
    
    // Build the property tree
    ptree pt;
    read_json(shellExpandPath("${SWATCH_ROOT}/system/test/cfg/system.json"), pt);
    XParameterSet sysset = swatch::system::treeToSystemPars(pt);  
    swatch::system::System* lSystem = swatch::core::Factory::get()->bake<swatch::system::System>(sysset);


    std::deque<swatch::processor::Processor*>& lProcessors( lSystem->getProcessors() );

    for( std::deque<swatch::processor::Processor*>::iterator lIt( lProcessors.begin()) ; lIt!=lProcessors.end() ; ++lIt )
    {
      const swatch::processor::ProcessorStub& stub = (**lIt).pset().get<swatch::processor::ProcessorBag>("stub").bag;

      std::cout << (**lIt).path() << std::endl;
      std::cout << stub << std::endl;

//       const std::string* lStr( NULL );
//       lStr = ( **lIt ).mConfigSequence.getHardwareType();
//       if( lStr ) std::cout << "HW : " << *lStr << std::endl;
//       else std::cout << "HW : NULL" << std::endl;
// 
//       lStr = ( **lIt ).mConfigSequence.getSystemId();
//       if( lStr ) std::cout << "System : " << *lStr << std::endl;
//       else std::cout << "System : NULL" << std::endl;
// 
//       lStr = ( **lIt ).mConfigSequence.getComponentId();
//       if( lStr ) std::cout << "Component : " << *lStr << std::endl;
//       else std::cout << "Component : NULL" << std::endl;

    }



    return 0;
}

