  /* 
 * File:   listParser.cpp
 * Author: ale
 *
 * Created on September 1, 2015, 5:06 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

using namespace std;


struct IntListGrammar : boost::spirit::qi::grammar<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type >
{
  IntListGrammar();
  
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > query;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > element;
  boost::spirit::qi::rule<std::string::const_iterator, int(), boost::spirit::ascii::space_type> literal;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<int>(), boost::spirit::ascii::space_type > range;
  
  static void push_back_range(std::vector<int>& v, int aFirst, int aLast);

};


IntListGrammar::IntListGrammar() :
  IntListGrammar::base_type(query)
{

  
  namespace qi = boost::spirit::qi;
  query = element % ',';

  element = ( range | literal );
  literal = qi::int_;
  range = (qi::int_ >> '-' >> qi::int_)[boost::phoenix::bind(&push_back_range, qi::_val, qi::_1, qi::_2) ];
  
  BOOST_SPIRIT_DEBUG_NODES((query));

}



 void IntListGrammar::push_back_range(std::vector<int>& v, int aFirst, int aLast) {

  boost::counting_iterator<int> lBegin(aFirst);
  boost::counting_iterator<int> lEnd(aLast);

  return v.insert(v.end(), lBegin, lEnd);

}
  

#include <boost/foreach.hpp>

/*
 * 
 */
int main(int argc, char** argv) {
  
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  
  std::string aaa = "1,2, 3,5-10";

  std::cout << "Kazabam! " << aaa << std::endl;

  std::string::const_iterator lBegin(aaa.begin()), lEnd(aaa.end());
  IntListGrammar lMyGrammar;
  
  std::vector<unsigned int> lIds;
  bool notDisaster = qi::phrase_parse ( lBegin , lEnd , lMyGrammar , ascii::space , lIds );
  if ( notDisaster ) {
    std::cout << "Parsing didn't fail: " << lIds.size() << std::endl;

    BOOST_FOREACH( unsigned int i, lIds) {
      std::cout << i << " ";
    }
    std::cout << std::endl;

  } else {
    std::cout << "pizello" << std::endl;
  }
}

