/* 
 * File:   IntListGrammar.hpp
 * Author: ale
 *
 * Created on September 15, 2015, 7:21 AM
 */

#ifndef __SWATCH_CORE_TOOLBOX_INTLISTPARSER_HPP__
#define	__SWATCH_CORE_TOOLBOX_INTLISTPARSER_HPP__

#include <vector>
#include <string>

namespace swatch {
namespace core {
namespace toolbox {

struct IntListParser {
  static std::vector<int32_t> parse( const std::string& aStringList );
};

struct UIntListParser {
  static std::vector<uint32_t> parse( const std::string& aStringList );
};

} // namespace tools
} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_TOOLBOX_INTLISTPARSER_HPP__ */

