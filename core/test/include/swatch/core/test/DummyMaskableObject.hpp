/**
 * @file   DummyMaskableObject.hpp
 * @author Tom Williams
 * @date   December 2015
 */

#ifndef SWATCH_CORE_TEST_DUMMYMASKABLEOBJECT_HPP
#define SWATCH_CORE_TEST_DUMMYMASKABLEOBJECT_HPP


#include "swatch/core/MaskableObject.hpp"

namespace swatch {
namespace core {
namespace test {


// Can't use MaskableObject instances in tests below since MonitorableObject::retrieveMetricValues is still pure virtual
class DummyMaskableObject : public MaskableObject {
public:
  explicit DummyMaskableObject(const std::string& aId);

  void retrieveMetricValues();
};


} // namespace test
} // namespace core
} // namespace swatch


#endif	/* SWATCH_CORE_TEST_DUMMYMASKABLEOBJECT_HPP */

