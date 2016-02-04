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


#include <set>

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
        std::ostringstream msg;
        std::ostream_iterator<char> out_it (msg,"");
        msg << "Slice parsing stopped at position " << std::distance(aStringSlice.begin(), lBegin) << " of string '" << aStringSlice << "'";

        throw SliceParsingFailed(msg.str());
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
        std::ostringstream msg;
        msg << "SliceList parsing stopped at position " << std::distance(aStringSlice.begin(), lBegin) << " of string '" << aStringSlice << "'"; 

        throw SliceListParsingFailed(msg.str());
      }

  } else {
    // TODO: Should throw here
    return std::vector<std::string>();
  }
  
  // Special case for empty strings. They are parsed into a 1-element vector with a single empty string.
  // If so return an empty vector
  if (lIds.size() == 1 && lIds.front().empty())
    lIds.clear();
      
  return lIds;
}


std::set<std::string> IdSliceParser::parseSet(const std::string& aStringSlice)
{
  std::vector<std::string> lIdList = parseList(aStringSlice);
  
  std::set<std::string> lIdSet(lIdList.begin(), lIdList.end());
  
  if ( lIdList.size() != lIdSet.size()) {
    throw SliceSetParsingDuplicatesFound();
  }
  
  return lIdSet;
}
    



} // namespace toolbox 
} // namespace core 
} // namespace swatch
