
#include <boost/test/unit_test.hpp>

// Swatch Headers
//#include "swatch/processor/Connection.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
// Namespace resolution
using std::cout;
using std::endl;

namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )

//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(CreationTest) {
  std::cout << "CreationTest" << std::endl;
  
  DummyProcessor* p = new DummyProcessor("dummy", DummyProcessor::generateParams());
  BOOST_CHECK_EQUAL(p->getSlot(), 0);
  BOOST_CHECK_EQUAL(p->getCrateId(), "s2g20-10");
  delete p;
}
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
