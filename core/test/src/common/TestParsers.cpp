/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
#include "boost/test/test_tools.hpp"
#include "swatch/core/toolbox/IdSliceGrammar.hpp"

#include <swatch/core/toolbox/IdSliceParser.hpp>

namespace swatch {
namespace core {
namespace test{

BOOST_AUTO_TEST_SUITE( ParserTestSuite )

BOOST_AUTO_TEST_CASE(IdSliceBasic) {

  std::string simple = "Rx01";
  
  std::vector<std::string> ids = toolbox::IdSliceParser::parse(simple);
  
  BOOST_CHECK_EQUAL( ids.size(), (size_t)1);
  BOOST_CHECK_EQUAL( ids[0], "Rx01");
}

BOOST_AUTO_TEST_CASE(IdSliceRange) {

  std::string simple = "Rx[01:05]";
  std::vector<std::string> expected = {"Rx01","Rx02","Rx03","Rx04"};
  
  std::vector<std::string> ids = toolbox::IdSliceParser::parse(simple);
  
  BOOST_CHECK_EQUAL( ids.size(), (size_t)4);
  BOOST_CHECK_EQUAL_COLLECTIONS(ids.begin(), ids.end(), 
                              expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(IdSliceRangePostfix) {

  std::string simple = "Rx[01:05]post";
  std::vector<std::string> expected = {"Rx01post","Rx02post","Rx03post","Rx04post"};
  
  std::vector<std::string> ids = toolbox::IdSliceParser::parse(simple);
  
  BOOST_CHECK_EQUAL( ids.size(), (size_t)4);
  BOOST_CHECK_EQUAL_COLLECTIONS(ids.begin(), ids.end(), 
                              expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(IdSliceRangeException) {

  std::string simple = "Rx[05:01]";
  BOOST_CHECK_THROW(toolbox::IdSliceParser::parse(simple), toolbox::InvalidSliceRange);
  
}


BOOST_AUTO_TEST_SUITE_END() // ObjectTestSuite

} //namespace test
} //namespace core
} //namespace swatch