/* 
 * File:   IntListGrammar.hpp
 * Author: ale
 *
 * Created on September 15, 2015, 9:23 AM
 */

#ifndef __SWATCH_CORE_TOOLBOX_INTLISTGRAMMAR_HPP__
#define	__SWATCH_CORE_TOOLBOX_INTLISTGRAMMAR_HPP__

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace swatch {
namespace core {
namespace toolbox {

struct IntListGrammar : boost::spirit::qi::grammar<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type >
{
  IntListGrammar();
  
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > mQuery;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > mElement;
  boost::spirit::qi::rule<std::string::const_iterator, int(), boost::spirit::ascii::space_type> mLiteral;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > mRange;
  
  static void push_back_range(std::vector<int>& aVector, int aFirst, int aLast);

};

} // namespace tools
} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_TOOLBOX_INTLISTGRAMMAR_HPP__ */

