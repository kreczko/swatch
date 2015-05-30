// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
#include "boost/test/test_tools.hpp"

// Swatch Headers
#include "swatch/core/StatusFlag.hpp"

// Boost Headers
#include <boost/lexical_cast.hpp>


BOOST_AUTO_TEST_SUITE( StatusFlagTestSuite )

BOOST_AUTO_TEST_CASE(StatusFlagStreamNoNamespace) {
  
  BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(swatch::core::kError) , "ERROR" );

  std::ostringstream oss;
  oss << swatch::core::kError;
  BOOST_CHECK_EQUAL( oss.str(), "ERROR" );
} 


BOOST_AUTO_TEST_CASE(StatusFlagStreamUsingNamespaceSwatchCore) {
  using namespace swatch::core;
  
  BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(swatch::core::kError) , "ERROR" );

  std::ostringstream oss;
  oss << swatch::core::kError;
  BOOST_CHECK_EQUAL( oss.str(), "ERROR" );
} 


namespace test_arbitrary {

BOOST_AUTO_TEST_CASE(StatusFlagStreamInArbitraryNamespace) {

  BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(swatch::core::kError) , "ERROR" );
  
  std::ostringstream oss;
  oss << swatch::core::kError;
  BOOST_CHECK_EQUAL( oss.str(), "ERROR" );
}

}



BOOST_AUTO_TEST_SUITE_END() // StatusFlagTestSuite
        

