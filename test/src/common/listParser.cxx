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

// Boost Headers
#include <boost/lexical_cast.hpp>
//#include <boost/spirit/include/qi.hpp>
////#include <boost/spirit/include/qi_grammar.hpp>
//#include <boost/iterator/counting_iterator.hpp>
//#include <boost/spirit/include/phoenix_core.hpp>
//#include <boost/spirit/include/phoenix_bind.hpp>
//#include <boost/spirit/include/phoenix_operator.hpp>
////#include <boost/spirit/include/phoenix.hpp>
//#include <boost/spirit/include/phoenix_statement.hpp>

// C++ Headers
#include <iomanip>

#include "swatch/core/toolbox/IntListGrammar.hpp"
#include "swatch/core/toolbox/IdSliceGrammar.hpp"

using namespace std;


#include <boost/foreach.hpp>


/*
 * 
 */
int main(int argc, char** argv)
{

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  // Test1
  {
    std::string aaa = "1,2, 3,5-10";

    std::cout << "Kazabam! " << aaa << std::endl;

    std::string::const_iterator lBegin(aaa.begin()), lEnd(aaa.end());
    swatch::core::toolbox::IntListGrammar lMyGrammar;

    std::vector<unsigned int> lIds;
    //		bool notDisaster = qi::parse ( lBegin , lEnd , lMyGrammar , lIds );
    bool notDisaster = qi::phrase_parse(lBegin, lEnd, lMyGrammar, ascii::space, lIds);
    if (notDisaster) {
      std::cout << "Parsing didn't fail: " << lIds.size() << std::endl;


      BOOST_FOREACH(unsigned int i, lIds)
      {
        std::cout << i << " ";
      }
      std::cout << std::endl;

    } else {
      std::cout << "pizello" << std::endl;
    }
  }

  // Test2
  {
    std::string bbb = "Rx10,Tx11, Tx[01:15], zz[9:14]xx, kk[1:10:5], oo[10:1:-2]";

    std::cout << "Kazabum! " << bbb << std::endl;

    std::string::const_iterator lBegin(bbb.begin()), lEnd(bbb.end());
    swatch::core::toolbox::IdSliceGrammar lMyGrammar;

    std::vector<std::string> lIds;
    bool notDisaster = qi::phrase_parse(lBegin, lEnd, lMyGrammar, ascii::space, lIds);
    if (notDisaster) {
      std::cout << "Parsing didn't fail: " << lIds.size() << " obj generated" << std::endl;
      std::cout << *lBegin << " -> "  << *lEnd << std::endl;

      BOOST_FOREACH(std::string i, lIds)
      {
        std::cout << i << " ";
      }
      std::cout << std::endl;

      if (lBegin != lEnd) {
        std::cout << "Warning: part of the phrase was not parsed" << std::endl;
        std::ostream_iterator<char> out_it(std::cout, "");
        std::copy(lBegin, lEnd, out_it);
        std::cout << std::endl;
      }
    } else {
      std::cout << "pizello" << std::endl;
    }


  }

}

