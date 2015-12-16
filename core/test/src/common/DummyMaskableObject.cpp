/**
 * @file   DummyMaskableObject.cpp
 * @author Tom Williams
 * @date   December 2015
 */


#include "swatch/core/test/DummyMaskableObject.hpp"



namespace swatch {
namespace core {
namespace test {


DummyMaskableObject::DummyMaskableObject(const std::string& aId) :
  MaskableObject(aId)
{
}

void DummyMaskableObject::retrieveMetricValues() {}


} // namespace test
} // namespace core
} // namespace swatch

