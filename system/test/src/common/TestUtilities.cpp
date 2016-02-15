
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
  
  boost::property_tree::ptree lPTree;
  lPTree.add<std::string>("NAME", "myLink_[0:10]");
  lPTree.add<std::string>("RX", "mySrc");
  lPTree.add<std::string>("RX PORT", "mySrcPort_[0:20:2]");
  lPTree.add<std::string>("TX", "myDst");
  lPTree.add<std::string>("TX PORT", "myDstPort_[200:0:-20]");
  
  std::vector<swatch::system::LinkStub> result;
  std::vector<swatch::system::LinkStub> expected;
  
  treeToLinkStub(lPTree, result);
  // Generate expected vector
  size_t j = 0;   // j : src port
  size_t k = 200; // k : dst port
  for(size_t i=0; i<10; i++, j+=2, k-=20)
  {
    std::ostringstream oss;
    oss << "myLink_" << std::setw(2) << std::setfill('0') << i;
    
    LinkStub b(oss.str());

    b.srcProcessor = "mySrc";
    oss.str("");
    oss << "mySrcPort_" << std::setw(2) << std::setfill('0') << j;
    b.srcPort = oss.str();
    
    b.dstProcessor = "myDst";
    oss.str("");
    oss << "myDstPort_" << std::setw(3) << std::setfill('0') << k;
    b.dstPort = oss.str();
    
    expected.push_back(b);
  }
  
  BOOST_CHECK_EQUAL(result.size(), expected.size());
  for( size_t i = 0; i < std::min(result.size(), expected.size()); i++ )
  {
    BOOST_CHECK_EQUAL(std::string(result[i].id), std::string(expected[i].id));
    BOOST_CHECK_EQUAL(std::string(result[i].srcPort), std::string(expected[i].srcPort));
    BOOST_CHECK_EQUAL(std::string(result[i].dstPort), std::string(expected[i].dstPort));
  }

  result.clear();

  // Check for throwing when lengths of link name, source port and/or dest port sequences aren't equal
  boost::property_tree::ptree lPTreeA;
  lPTreeA.add<std::string>("NAME", "myLink_[0:2]");
  lPTreeA.add<std::string>("RX", "a_src_proc");
  lPTreeA.add<std::string>("RX PORT", "a_src_port[0:3]");
  lPTreeA.add<std::string>("TX", "a_dst_proc");
  lPTreeA.add<std::string>("TX PORT", "a_dst_port[0:3]");
  BOOST_CHECK_THROW(treeToLinkStub(lPTreeA, result), std::runtime_error);
  BOOST_CHECK( result.empty() );

  boost::property_tree::ptree lPTreeB;
  lPTreeB.add<std::string>("NAME", "myLink_[0:3]");
  lPTreeB.add<std::string>("RX", "a_src_proc");
  lPTreeB.add<std::string>("RX PORT", "a_src_port[0:2]");
  lPTreeB.add<std::string>("TX", "a_dst_proc");
  lPTreeB.add<std::string>("TX PORT", "a_dst_port[0:3]");
  BOOST_CHECK_THROW(treeToLinkStub(lPTreeB, result), std::runtime_error);
  BOOST_CHECK( result.empty() );

  boost::property_tree::ptree lPTreeC;
  lPTreeC.add<std::string>("NAME", "myLink_[0:3]");
  lPTreeC.add<std::string>("RX", "a_src_proc");
  lPTreeC.add<std::string>("RX PORT", "a_src_port[0:3]");
  lPTreeC.add<std::string>("TX", "a_dst_proc");
  lPTreeC.add<std::string>("TX PORT", "a_dst_port[0:2]");
  BOOST_CHECK_THROW(treeToLinkStub(lPTreeC, result), std::runtime_error);
  BOOST_CHECK( result.empty() );

}
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
