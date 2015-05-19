#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyFunctionoid.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>

using namespace swatch;
namespace swatch {
namespace core {
namespace test {

DummyFunctionoid::DummyFunctionoid(  const std::string& aId ) :
        core::Functionoid( aId ) {
}

DummyFunctionoid::~DummyFunctionoid() {
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
