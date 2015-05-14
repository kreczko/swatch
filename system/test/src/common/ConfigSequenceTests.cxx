/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>
#include <iostream>

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

    std::cout << *lSystem << std::endl;


    std::deque<swatch::processor::Processor*>& lProcessors( lSystem->getProcessors() );

    for( std::deque<swatch::processor::Processor*>::iterator lIt( lProcessors.begin()) ; lIt!=lProcessors.end() ; ++lIt )
    {
      std::set<std::string> lConfigSequences( (**lIt).getConfigSequences() );

      for( std::set<std::string>::iterator lIt2( lConfigSequences.begin()) ; lIt2!=lConfigSequences.end() ; ++lIt2 )
      {
        std::cout << *lIt2 << std::endl;
        swatch::core::ConfigSequence* lConfigSequence( (**lIt).getConfigSequence( *lIt2 ) );
      
        std::set<std::string> lParams = lConfigSequence->getParams();

        for( std::set<std::string>::iterator lIt3( lParams.begin()) ; lIt3!=lParams.end() ; ++lIt3 )
        {
          std::cout << "[Param] " << *lIt3 << std::endl;
        }

        std::deque<std::string> lTables = lConfigSequence->getTables();
        for( std::deque<std::string>::iterator lIt3( lTables.begin()) ; lIt3!=lTables.end() ; ++lIt3 )
        {
          std::cout << "[Table] " << *lIt3 << std::endl;
        }
      }

    }



    return 0;
}

