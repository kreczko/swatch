/* 
 * File:   IdSliceGrammar.hpp
 * Author: ale
 *
 * Created on September 15, 2015, 9:23 AM
 */

#ifndef __SWATCH_CORE_TOOLBOX_IDSLICEGRAMMAR_HPP__
#define __SWATCH_CORE_TOOLBOX_IDSLICEGRAMMAR_HPP__

// Boost Headers
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {
namespace toolbox {

//----- 
struct IdSliceGrammar : boost::spirit::qi::grammar<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > {
    IdSliceGrammar();
     
    boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mQuery;
    boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mElement;
    boost::spirit::qi::rule<std::string::const_iterator, std::string(), boost::spirit::ascii::space_type> mLiteral;
    boost::spirit::qi::rule<std::string::const_iterator, std::string(), boost::spirit::ascii::space_type> mNumber;
    boost::spirit::qi::rule<std::string::const_iterator, int32_t(), boost::spirit::ascii::space_type> mStep;
    boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mRange;

  static void push_back_range(std::vector<std::string>& aResult, const std::string& aPrefix, const std::string& aFirst, const std::string& aLast, const boost::optional<int32_t>& aStep, const boost::optional<std::string>& aPostfix);

};

DEFINE_SWATCH_EXCEPTION(InvalidSliceStep);
DEFINE_SWATCH_EXCEPTION(InvalidSliceRange);
 
} // namespace tools
} // namespace core
} // namespace swatch



#endif /* __SWATCH_CORE_TOOLBOX_IDSLICEGRAMMAR_HPP__ */

