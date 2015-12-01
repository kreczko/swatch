/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IdSliceGrammar.cpp
 * Author: ale
 * 
 * Created on November 16, 2015, 4:25 PM
 */

#include "swatch/core/toolbox/IdSliceGrammar.hpp"

// Boost Headers
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

// C++ Headers
#include <iomanip>

namespace swatch {
namespace core {
namespace toolbox {

// --------------------------------------------------------
IdSliceGrammar::IdSliceGrammar() :
  IdSliceGrammar::base_type(mQuery) {
  
  namespace qi = boost::spirit::qi;
  namespace phx = boost::phoenix;

  mQuery = mElement % ',';
  mElement = (mRange | mLiteral);
  mLiteral = *(qi::char_ - ',' - '[' - ':' - ']');
  mNumber = *(qi::char_("0-9"));
//  mRange =
//      (mLiteral >> '[' >> mNumber >> ':' >> mNumber >> ']' >> -mLiteral)[
//        phx::try_ [
//          phx::bind(&push_back_range, qi::_val, qi::_1, qi::_4, qi::_2, qi::_3)
//        ] .catch_<InvalidSliceRange>() [
//          std::cout << phx::val(""),
//          phx::throw_()
//        ] .catch_all [
//          qi::_pass = false
//        ]
//      ];

  
  
  mRange =
      (mLiteral >> '[' >> mNumber >> ':' >> mNumber >> ':' >> mNumber >> ']' >> mLiteral)[
        phx::bind(&push_back_range, qi::_val, qi::_1, qi::_5, qi::_2, qi::_3, qi::_4)
      ];
}


// --------------------------------------------------------
void IdSliceGrammar::push_back_range(std::vector<std::string>& aResult, const std::string& aPrefix, const std::string& aPostfix, const std::string& aFirst, const std::string& aLast, const std::string& aStep) {
    
    const size_t firstIdx = boost::lexical_cast<size_t>(aFirst);
    const size_t lastIdx = boost::lexical_cast<size_t>(aLast);
    const size_t idStep = boost::lexical_cast<size_t>(aStep);

    const size_t idxWidth = std::max(aFirst.size(), aLast.size());
    
    if ( firstIdx > lastIdx ) 
        throw InvalidSliceRange(
            "First index ("+boost::lexical_cast<std::string>(aFirst)+
            ") bigger than last ("+boost::lexical_cast<std::string>(aLast)+")"
            );
            
    std::ostringstream oss;
    
    for( size_t idx(firstIdx); idx<lastIdx; idx += idStep) {
        oss.str("");
        oss << aPrefix << std::setw(idxWidth) << std::setfill('0') << idx << aPostfix;
        aResult.push_back( oss.str());
    }
    
}


} // namespace toolbox 
} // namespace core 
} // namespace swatch

