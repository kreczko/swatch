
#include <boost/test/unit_test.hpp>

// Swathc Headers
#include "swatch/processor/test/SillyFSM.hpp"

// Namespace resolution
using std::cout;
using std::endl;

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )
        
//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(FSMTest) {
    using namespace swatch::processor::test;

    SillyFSM fsm;
    fsm.setInitialState("Initial");
    BOOST_CHECK(fsm.getInitialState() == "Initial");
    fsm.reset();
    BOOST_CHECK(fsm.getCurrentState() == "Initial");
    
    


}
        
        
        
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite