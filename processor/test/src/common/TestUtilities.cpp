
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/processor/Utilities.hpp"


namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )

//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(SliceSyntaxParsingTests) {
  std::cout << "ProcessorTestSuite.SliceSyntaxParsingTests" << std::endl;
  std::vector<std::string> expected, returned;
  
  
  // Very basic strings (no prefix / suffix)
  expected = {"0", "1"};
  returned = expandPortSliceSyntax("[0:2]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  returned = expandPortSliceSyntax("[0:2:1]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  expected = {"0", "3", "6"};
  returned = expandPortSliceSyntax("[0:7:3]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  returned = expandPortSliceSyntax("[0:9:3]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  expected = {"12", "24", "36"};
  returned = expandPortSliceSyntax("[12:37:12]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  returned = expandPortSliceSyntax("[12:48:12]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());

  
  // Very basic strings (no prefix / suffix), fixed width
  expected = {"00", "01"};
  returned = expandPortSliceSyntax("[00:2]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  returned = expandPortSliceSyntax("[0:02]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  
  // Strings with prefix + suffix
  expected = {"abc_rx00_def", "abc_rx05_def", "abc_rx10_def"};
  returned = expandPortSliceSyntax("abc_rx[0:15:5]_def");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  
  // Finally, strings that don't contain port slice syntax
  expected = {"abc[0]def"};
  returned = expandPortSliceSyntax("abc[0]def");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());

  expected = {"abc[0:2:3:4]def"};
  returned = expandPortSliceSyntax("abc[0:2:3:4]def");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  expected = {"[a:b:c]"};
  returned = expandPortSliceSyntax("[a:b:c]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());

}
  

BOOST_AUTO_TEST_CASE(SliceSyntaxPortStubTests){
  std::cout << "ProcessorTestSuite.SliceSyntaxPortStubTests" << std::endl;

  xdata::Vector<swatch::processor::ProcessorPortBag> result;
  std::vector<swatch::processor::ProcessorPortBag> expected;
  
  expandPortSliceSyntax("myPort_rx[0:10]", "[10:60:5]", result);
  // Generated expected vector (i used for port name, j is port number)
  size_t j=10;
  for(size_t i=0; i<10; i++, j+=5)
  {
    std::ostringstream oss;
    oss << "myPort_rx" << std::setw(2) << std::setfill('0') << i;
    
    ProcessorPortBag b;
    b.bag.name = oss.str();
    b.bag.number = j;   
    expected.push_back(b);
  }

  BOOST_CHECK_EQUAL(result.size(), expected.size());
  for( size_t i = 0; i < std::min(result.size(), expected.size()); i++ )
  {
    BOOST_CHECK_EQUAL(std::string(result[i].bag.name), std::string(expected[i].bag.name));
    BOOST_CHECK_EQUAL(unsigned(result[i].bag.number), unsigned(expected[i].bag.number));
  }

  
  result.clear();
  // Check for throwing when lengths of name and index sequence aren't equal
  BOOST_CHECK_THROW(expandPortSliceSyntax("myPort_rx[0:3]", "[0:2]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
  // Check that exception thrown when the expanded port index strings can't be cast to an unsigned
  BOOST_CHECK_THROW(expandPortSliceSyntax("myPort_rx[0:3]", "a[0:2]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
}
    
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
