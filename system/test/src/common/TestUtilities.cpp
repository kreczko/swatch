
#include <boost/test/unit_test.hpp>

// standard headers
#include <iomanip>

// SWATCH headers
#include "swatch/system/Utilities.hpp"


namespace swatch {
namespace system {
namespace test {

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_AUTO_TEST_CASE(SliceSyntaxLinkStubTests) {
  std::cout << "ProcessorTestSuite.SliceSyntaxLinkStubTests" << std::endl;
  
  std::vector<swatch::system::LinkStub> result;
  std::vector<swatch::system::LinkStub> expected;
  
  pushBackLinkStubs(result, "myLink_[0:10]", "mySrcPort_[0:20:2]", "myDstPort_[200:0:-20]");
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

  BOOST_CHECK_THROW(pushBackLinkStubs(result, "myLink_[0:2]", "a_src_port[0:3]", "a_dst_port[0:3]"), std::runtime_error);
  BOOST_CHECK( result.empty() );
  
  BOOST_CHECK_THROW(pushBackLinkStubs(result, "myLink_[0:3]", "a_src_port[0:2]", "a_dist_port[0:3]"), std::runtime_error);
  BOOST_CHECK( result.empty() );

  BOOST_CHECK_THROW(pushBackLinkStubs(result, "myLink_[0:3]", "a_src_port[0:3]", "a_dist_port[0:2]"), std::runtime_error);
  BOOST_CHECK( result.empty() );

}
    
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
