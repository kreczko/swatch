/**
 * @file    DummyFunctionoid.hpp
 * @author  Luke Kreczko
 * @brief   Functionoid class
 * @date    May 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYFUNCTIONOID_HPP
#define SWATCH_CORE_TEST_DUMMYFUNCTIONOID_HPP

#include "swatch/core/Functionoid.hpp"
#include "swatch/core/ActionableObject.hpp"
#include <xdata/Integer.h>
#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {
class DummyFunctionoid: public swatch::core::Functionoid {
public:
  DummyFunctionoid( const std::string& aId );
  virtual ~DummyFunctionoid();

  template<typename T>
  void registerParam(const std::string name, const T& defaultValue) {
    core::Functionoid::registerParameter(name, defaultValue);
  }


};

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYFUNCTIONOID_HPP */
