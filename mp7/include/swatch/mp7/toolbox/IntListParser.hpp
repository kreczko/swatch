/* 
 * File:   IntListGrammar.hpp
 * Author: ale
 *
 * Created on September 15, 2015, 7:21 AM
 */

#ifndef SWATCH_MP7_TOOLBOX_INTLISTPARSER_HPP
#define	SWATCH_MP7_TOOLBOX_INTLISTPARSER_HPP

#include <vector>
#include <string>

namespace swatch {
namespace mp7 {
namespace toolbox {

struct IntListParser {
  static std::vector<int32_t> parse( const std::string& aStringList );
};

struct UIntListParser {
  static std::vector<uint32_t> parse( const std::string& aStringList );
};

} // namespace tools
} // namespace mp7
} // namespace swatch


#endif	/* SWATCH_MP7_TOOLBOX_INTLISTPARSER_HPP */

