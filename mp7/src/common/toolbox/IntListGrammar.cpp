#include "swatch/mp7/toolbox/IntListGrammar.hpp"

#include <boost/iterator/counting_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>


namespace swatch {
namespace mp7 {
namespace toolbox {

// --------------------------------------------------------
IntListGrammar::IntListGrammar() :
  IntListGrammar::base_type(query) {


  namespace qi = boost::spirit::qi;
  query = element % ',';

  element = (range | literal);
  literal = qi::int_;
  range = (qi::int_ >> '-' >> qi::int_)[boost::phoenix::bind(&push_back_range, qi::_val, qi::_1, qi::_2) ];

  //  BOOST_SPIRIT_DEBUG_NODES((query));

}


// --------------------------------------------------------
void IntListGrammar::push_back_range(std::vector<int>& v, int aFirst, int aLast) {

  boost::counting_iterator<int> lBegin(aFirst);
  boost::counting_iterator<int> lEnd(aLast);

  return v.insert(v.end(), lBegin, lEnd);

}


} // namespace toolbox 
} // namespace mp7 
} // namespace swatch

