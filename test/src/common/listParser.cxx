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
#include <boost/spirit/include/qi.hpp>
//#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>

// C++ Headers
#include <iomanip>


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
//----- 
 struct IdSliceGrammar : boost::spirit::qi::grammar<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > {
	 IdSliceGrammar();
	 
	boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mQuery;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mElement;
	boost::spirit::qi::rule<std::string::const_iterator, std::string(), boost::spirit::ascii::space_type> mLiteral;
	boost::spirit::qi::rule<std::string::const_iterator, std::string(), boost::spirit::ascii::space_type> mNumber;
  boost::spirit::qi::rule<std::string::const_iterator, std::vector<std::string>(), boost::spirit::ascii::space_type > mRange;

  static void push_back_range(std::vector<std::string>& aResult, const std::string& aBasename, const std::string& aFirst, const std::string& aLast);

 };
 
 IdSliceGrammar::IdSliceGrammar() :
  IdSliceGrammar::base_type(mQuery) {
	namespace qi = boost::spirit::qi;
	namespace phx = boost::phoenix;

  mQuery = mElement % ',';
	mElement = ( mRange | mLiteral );
	mLiteral = *(qi::char_ - ',' - '[' - ':' - ']');
	mNumber = *(qi::char_("0-9"));
	mRange = 
			(mLiteral >> '['>> mNumber >> ':' >> mNumber >> ']')[
				phx::try_ [
					phx::bind(&push_back_range, qi::_val, qi::_1, qi::_2, qi::_3)
			] .catch_<runtime_error>() [
			  std::cout << phx::val("caught runtime error or derived\n"),
				phx::throw_()
			] .catch_all [ 
				qi::_pass = false 
			]
			];

}
 
void IdSliceGrammar::push_back_range(std::vector<std::string>& aResult, const std::string& aBasename, const std::string& aFirst, const std::string& aLast) {
	
	const size_t firstIdx = boost::lexical_cast<size_t>(aFirst);
	const size_t lastIdx = boost::lexical_cast<size_t>(aLast);
	const size_t idxWidth = std::max(aFirst.size(), aLast.size());
	
	if ( firstIdx > lastIdx ) 
		throw std::runtime_error("puppa");
	
	std::ostringstream oss;
	
	for( size_t idx(firstIdx); idx<lastIdx; ++idx) {
		oss.str("");
		oss << aBasename << std::setw(idxWidth) << std::setfill('0') << idx;
		aResult.push_back( oss.str());
	}
	
}

#include <boost/foreach.hpp>

/*
 * 
 */
int main(int argc, char** argv) {
  
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  
	// Test1
	{ 
		std::string aaa = "1,2, 3,5-10";

		std::cout << "Kazabam! " << aaa << std::endl;

		std::string::const_iterator lBegin(aaa.begin()), lEnd(aaa.end());
		IntListGrammar lMyGrammar;

		std::vector<unsigned int> lIds;
//		bool notDisaster = qi::parse ( lBegin , lEnd , lMyGrammar , lIds );
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
	
	// Test2
	{
	std::string bbb = "Rx10,Tx11, Tx[01:15], zz[9:4]";
	
	std::cout << "Kazabum! " << bbb << std::endl;
	
	std::string::const_iterator lBegin(bbb.begin()), lEnd(bbb.end());
	IdSliceGrammar lMyGrammar;

	std::vector<std::string> lIds;
	bool notDisaster = qi::phrase_parse ( lBegin , lEnd , lMyGrammar , ascii::space , lIds );
	if ( notDisaster ) {
		std::cout << "Parsing didn't fail: " << lIds.size() << std::endl;

		BOOST_FOREACH( std::string i, lIds) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
		

		if ( lBegin != lEnd ) {
			std::cout << "Warning: part of the phrase was not parsed" << std::endl; 
			std::ostream_iterator<char> out_it (std::cout,"");
			std::copy ( lBegin, lEnd, out_it );
			std::cout << std::endl;
			
			
		}
		} else {
			std::cout << "pizello" << std::endl;
		}
		
	
	}
	
}

