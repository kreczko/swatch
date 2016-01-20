
#include <boost/test/unit_test.hpp>

// standard headers
#include <iomanip>

// SWATCH headers
#include "swatch/processor/Utilities.hpp"


namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )


BOOST_AUTO_TEST_CASE(SliceSyntaxPortStubTests){
  std::cout << "ProcessorTestSuite.SliceSyntaxPortStubTests" << std::endl;

  std::vector<swatch::processor::ProcessorPortStub> result;
  std::vector<swatch::processor::ProcessorPortStub> expected;
  
//  expandPortSliceSyntax("myPort_rx[0:10]", "[10:60:5]", result);
  pushBackPortStubs(result, "myPort_rx[0:10]", "[10:60:5]");

  // Generated expected vector (i used for port name, j is port number)
  size_t j=10;
  for(size_t i=0; i<10; i++, j+=5)
  {
    std::ostringstream msg;
    msg << "myPort_rx" << std::setw(2) << std::setfill('0') << i;
    
    ProcessorPortStub b(msg.str());
    b.number = j;   
    expected.push_back(b);
  }

  BOOST_CHECK_EQUAL(result.size(), expected.size());
  for( size_t i = 0; i < std::min(result.size(), expected.size()); i++ )
  {
    BOOST_CHECK_EQUAL(std::string(result[i].id), std::string(expected[i].id));
    BOOST_CHECK_EQUAL(unsigned(result[i].number), unsigned(expected[i].number));
  }

  
  result.clear();
  
  // Check for throwing when lengths of name and index sequence aren't equal
  BOOST_CHECK_THROW(pushBackPortStubs(result, "myPort_rx[0:3]", "[0:2]"), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
  // Check that exception thrown when the expanded port index strings can't be cast to an unsigned
  BOOST_CHECK_THROW(pushBackPortStubs(result, "myPort_rx[0:3]", "a[0:2]"), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
}

        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
