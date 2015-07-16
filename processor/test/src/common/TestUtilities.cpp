
#include <boost/test/unit_test.hpp>

// standard headers
#include <iomanip>

// SWATCH headers
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


  // Very basic strings, decreasing numbers
  expected = {"12", "11", "10"};
  returned = expandPortSliceSyntax("[12:9:-1]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  
  expected = {"12", "09", "06"};
  returned = expandPortSliceSyntax("[12:3:-3]");
  BOOST_CHECK_EQUAL_COLLECTIONS(returned.begin(), returned.end(), expected.begin(), expected.end());
  returned = expandPortSliceSyntax("[12:5:-3]");
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

  std::vector<swatch::processor::ProcessorPortStub> result;
  std::vector<swatch::processor::ProcessorPortStub> expected;
  
  expandPortSliceSyntax("myPort_rx[0:10]", "[10:60:5]", result);
  // Generated expected vector (i used for port name, j is port number)
  size_t j=10;
  for(size_t i=0; i<10; i++, j+=5)
  {
    std::ostringstream oss;
    oss << "myPort_rx" << std::setw(2) << std::setfill('0') << i;
    
    ProcessorPortStub b(oss.str());
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
  BOOST_CHECK_THROW(expandPortSliceSyntax("myPort_rx[0:3]", "[0:2]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
  // Check that exception thrown when the expanded port index strings can't be cast to an unsigned
  BOOST_CHECK_THROW(expandPortSliceSyntax("myPort_rx[0:3]", "a[0:2]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
}


BOOST_AUTO_TEST_CASE(SliceSyntaxLinkStubTests) {
  std::cout << "ProcessorTestSuite.SliceSyntaxLinkStubTests" << std::endl;
  
  std::vector<swatch::processor::LinkStub> result;
  std::vector<swatch::processor::LinkStub> expected;
  
  expandLinkSliceSyntax("myLink_[0:10]", "mySrcPort_[0:20:2]", "myDstPort_[200:0:-20]", result);
  // Generate expected vector
  size_t j = 0;   // j : src port
  size_t k = 200; // k : dst port
  for(size_t i=0; i<10; i++, j+=2, k-=20)
  {
    std::ostringstream oss;
    oss << "myLink_" << std::setw(2) << std::setfill('0') << i;
    
    LinkStub b(oss.str());

    oss.str("");
    oss << "mySrcPort_" << std::setw(2) << std::setfill('0') << j;
    b.src = oss.str();
    
    oss.str("");
    oss << "myDstPort_" << std::setw(3) << std::setfill('0') << k;
    b.dst = oss.str();
    
    expected.push_back(b);
  }
  
  BOOST_CHECK_EQUAL(result.size(), expected.size());
  for( size_t i = 0; i < std::min(result.size(), expected.size()); i++ )
  {
    BOOST_CHECK_EQUAL(std::string(result[i].id), std::string(expected[i].id));
    BOOST_CHECK_EQUAL(std::string(result[i].src), std::string(expected[i].src));
    BOOST_CHECK_EQUAL(std::string(result[i].dst), std::string(expected[i].dst));
  }



  result.clear();

  // Check for throwing when lengths of link name, source port and/or dest port sequences aren't equal

  BOOST_CHECK_THROW(expandLinkSliceSyntax("myLink_[0:2]", "a_src_port[0:3]", "a_dst_port[0:3]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
  BOOST_CHECK_THROW(expandLinkSliceSyntax("myLink_[0:3]", "a_src_port[0:2]", "a_dist_port[0:3]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );

  BOOST_CHECK_THROW(expandLinkSliceSyntax("myLink_[0:3]", "a_src_port[0:3]", "a_dist_port[0:2]", result), std::runtime_error);
  BOOST_CHECK( result.empty() );

}
    
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
