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
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"

#include "swatch/xml/XmlGateKeeper.hpp"
//#include "swatch/core/test/DummyGateKeeper.hpp"

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


#include <xdata/Integer.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>


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
    swatch::system::SystemStub sysstub = swatch::system::treeToSystemPars(pt);  
    swatch::system::System* lSystem = swatch::core::Factory::get()->make<swatch::system::System>(sysstub.creator, sysstub);

    std::cout << *lSystem << std::endl;

//    swatch::core::test::DummyGateKeeper lGateKeeper( lSystem );
    swatch::xml::XmlGateKeeper lGateKeeper( "${SWATCH_ROOT}/system/test/cfg/db.xml" , "RunKey1" );
//    lGateKeeper.preload();

    std::cout << lGateKeeper << std::endl;

    const std::deque<swatch::processor::Processor*>& lProcessors( lSystem->getProcessors() );

    for( std::deque<swatch::processor::Processor*>::const_iterator lIt( lProcessors.begin()) ; lIt!=lProcessors.end() ; ++lIt )
    {
      std::set<std::string> lCommandSequences( (**lIt).getSequences() );

      for( std::set<std::string>::iterator lIt2( lCommandSequences.begin()) ; lIt2!=lCommandSequences.end() ; ++lIt2 )
      {
        std::cout << std::string( 100 , '-' ) << std::endl;
        std::cout << *lIt2 << std::endl;
        swatch::core::CommandSequence& lCommandSequence( (**lIt).getSequence( *lIt2 ) );
      
//         std::set<std::string> lParams = lCommandSequence->getParams();
// 
//         for( std::set<std::string>::iterator lIt3( lParams.begin()) ; lIt3!=lParams.end() ; ++lIt3 )
//         {
//           std::cout << "[Param] " << *lIt3 << std::endl;
//         }
// 
//         std::vector<std::string> lTables = lCommandSequence->getTables();
//         for( std::vector<std::string>::iterator lIt3( lTables.begin()) ; lIt3!=lTables.end() ; ++lIt3 )
//         {
//           std::cout << "[Table] " << *lIt3 << std::endl;
//         }


   
        // for now just empty params
//         XParameterSet params;
//         params.add( "sequence_1.dummy_1.aa" , xdata::Integer(123) );
//         params.add( "sequence_1.dummy_2.aa" , xdata::Integer(321) );

        lCommandSequence.exec(lGateKeeper);
//         std::cout << lCommandSequence->getProgress() << std::endl;
//         std::cout << lCommandSequence->getOverallProgress() << std::endl;
//         std::cout << lCommandSequence->getProgressMsg() << std::endl;
//         std::cout << lCommandSequence->getStatusMsg() << std::endl;

      }

    }



    return 0;
}

