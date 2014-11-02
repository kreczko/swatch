
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
    BOOST_REQUIRE_THROW(fsm.reset(),std::runtime_error);
    fsm.addState("Initial");
    
    fsm.setInitialState("Initial");
    BOOST_CHECK(fsm.getInitialState() == "Initial");
    
    fsm.reset();
    BOOST_CHECK(fsm.getCurrentState() == "Initial");
    
    BOOST_REQUIRE_THROW(fsm.addStateTransition("Here","There","Go"),std::runtime_error);

    fsm.addState("Here");
    BOOST_REQUIRE_THROW(fsm.addStateTransition("Here","There","Go"),std::runtime_error);
    fsm.addState("There");

    fsm.addStateTransition("Here","There","Go");


    fsm.addStateTransition("Initial","Here","Come");

    fsm.execute("Come");
    BOOST_CHECK(fsm.getCurrentState() == "Here");
    
    fsm.execute("Go");
    BOOST_CHECK(fsm.getCurrentState() == "There");
}
        
        
        
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite