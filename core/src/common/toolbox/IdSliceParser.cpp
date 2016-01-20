/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IdSliceParser.cpp
 * Author: ale
 * 
 * Created on November 16, 2015, 4:58 PM
 */

#include "swatch/core/toolbox/IdSliceParser.hpp"

// SWATCH Heaers
#include "swatch/core/toolbox/IdSliceGrammar.hpp"

namespace swatch {
namespace core {
namespace toolbox {

// --------------------------------------------------------
std::vector<std::string>
IdSliceParser::parse(const std::string& aStringSlice) {
  
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  
  std::string::const_iterator lBegin(aStringSlice.begin()), lEnd(aStringSlice.end());

  IdSliceGrammar lGrammar;
  
  std::vector<std::string> lIds;
  bool success = qi::phrase_parse ( lBegin , lEnd , lGrammar , ascii::space , lIds );
  if ( success ) {

      // Throw if slice not fully parsed
      if ( lBegin != lEnd ) {
          std::cout << "Warning: part of the phrase was not parsed" << std::endl; 
          std::ostream_iterator<char> out_it (std::cout,"");
          std::copy ( lBegin, lEnd, out_it );
          std::cout << std::endl;
      }

  } else {
    // TODO: Should throw here
    return std::vector<std::string>();
  }
      
  return lIds;
}


std::vector<std::string> 
IdSliceParser::parseList(const std::string& aStringSlice)
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  
  std::string::const_iterator lBegin(aStringSlice.begin()), lEnd(aStringSlice.end());

  IdSliceListGrammar lGrammar;
  
  std::vector<std::string> lIds;
  bool success = qi::phrase_parse ( lBegin , lEnd , lGrammar , ascii::space , lIds );
  if ( success ) {

      // Throw if slice not fully parsed
      if ( lBegin != lEnd ) {
          std::cout << "Warning: part of the phrase was not parsed" << std::endl; 
          std::ostream_iterator<char> out_it (std::cout,"");
          std::copy ( lBegin, lEnd, out_it );
          std::cout << std::endl;
      }

  } else {
    // TODO: Should throw here
    return std::vector<std::string>();
  }
      
  return lIds;
}



} // namespace toolbox 
} // namespace core 
} // namespace swatch
