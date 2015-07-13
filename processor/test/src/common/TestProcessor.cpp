
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/processor/LinkInterface.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"



namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )

//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(CreationTest) {
  std::cout << "ProcessorTestSuite.CreationTest" << std::endl;
  
  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  BOOST_CHECK_EQUAL(p.getSlot(), uint32_t(0));
  BOOST_CHECK_EQUAL(p.getCrateId(), "s2g20-10");

  // Input and output ports
  BOOST_CHECK_EQUAL(p.linkInterface().getNumInputs(), 5);
  BOOST_CHECK_NO_THROW(p.linkInterface().getInput("rxA"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getInput("rxB"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getInput("rxC"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getInput("rxD"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getInput("rxE"));

  BOOST_CHECK_EQUAL(p.linkInterface().getNumOutputs(), 3);
  BOOST_CHECK_NO_THROW(p.linkInterface().getOutput("txA"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getOutput("txB"));
  BOOST_CHECK_NO_THROW(p.linkInterface().getOutput("txC"));
}
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
