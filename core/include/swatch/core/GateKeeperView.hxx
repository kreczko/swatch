/**
 * @file    GateKeeperView.hxx
 * @author  Luke Kreczko
 * @date    February 2016
 */

#ifndef __SWATCH_CORE_GATEKEEPERVIEW_HXX__
#define	__SWATCH_CORE_GATEKEEPERVIEW_HXX__

#include <string>

#include <boost/foreach.hpp>

#include "swatch/core/GateKeeperView.hpp"

namespace swatch {
namespace core {
template<typename T>
const std::vector<std::string> GateKeeperView::extractMapKeys(const boost::unordered_map<std::string, T>& aMap) const {
  std::vector<std::string> lKeys;
  // the BOOST_FOREACH macro is not happy when using th full type, hence typedef
  typedef std::pair<std::string, T> Pair_t;
  BOOST_FOREACH(const Pair_t lIt, aMap) {
    lKeys.push_back(lIt.first);
  }

  return lKeys;
}
} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_GATEKEEPERVIEW_HPP__ */

