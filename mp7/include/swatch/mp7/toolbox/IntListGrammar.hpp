/* 
 * File:   IntListGrammar.hpp
 * Author: ale
 *
 * Created on September 15, 2015, 9:23 AM
 */

#ifndef SWATCH_MP7_TOOLBOX_INTLISTGRAMMAR_HPP
#define	SWATCH_MP7_TOOLBOX_INTLISTGRAMMAR_HPP

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_grammar.hpp>

namespace swatch {
namespace mp7 {
namespace toolbox {

struct IntListGrammar : boost::spirit::qi::grammar<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type >
{
  IntListGrammar();
  
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > query;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > element;
  boost::spirit::qi::rule<std::string::const_iterator, int(), boost::spirit::ascii::space_type> literal;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > range;
  
  static void push_back_range(std::vector<int>& v, int aFirst, int aLast);

};

} // namespace tools
} // namespace mp7
} // namespace swatch


#endif	/* SWATCH_MP7_TOOLBOX_INTLISTGRAMMAR_HPP */

