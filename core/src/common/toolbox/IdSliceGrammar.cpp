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
  // mStep = *(qi::char_("0-9"));
  mStep = qi::int_;
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
      (mLiteral >> '[' >> mNumber >> ':' >> mNumber >> -(':' >> mStep) >> ']' >> mLiteral )[
        phx::bind(&push_back_range, qi::_val, qi::_1, qi::_2, qi::_3, qi::_4, qi::_5)
      ];
}


// --------------------------------------------------------
void IdSliceGrammar::push_back_range(std::vector<std::string>& aResult, const std::string& aPrefix, const std::string& aFirst, const std::string& aLast, const boost::optional<int32_t>& aStep, const boost::optional<std::string>& aPostfix)
{
    
    const int32_t lFirstId = boost::lexical_cast<int32_t>(aFirst);
    const int32_t lLastId = boost::lexical_cast<int32_t>(aLast);
    const int32_t lStep = (aStep ? *aStep : 1);

    const size_t lWidth = std::max(aFirst.size(), aLast.size());
    
    // Step 0? Not good 
    if ( !lStep )  {
      throw InvalidSliceStep("Step=0 not allowed");
    }

    // Check that the loop is not infinite 
    if ( (lFirstId < lLastId) ^ (lStep > 0 ) ) 
        throw InvalidSliceRange(
            "First index ("+boost::lexical_cast<std::string>(aFirst)+
            ") bigger than last ("+boost::lexical_cast<std::string>(aLast)+")"
            );

            
    std::ostringstream oss;
    

    for( int32_t i(lFirstId); (lLastId-i)/(float)lStep > 0 ; i += lStep) {
        // Reset stream
        oss.str("");
        oss << aPrefix << std::setw(lWidth) << std::setfill('0') << i;
        if (aPostfix)
          oss << *aPostfix;

        aResult.push_back( oss.str());

        // std::cout << "(" << lLastId << "-" << i <<")/" << lStep << "=" << (lLastId-i)/(float)lStep << std::endl;
    }
    
}


} // namespace toolbox 
} // namespace core 
} // namespace swatch

