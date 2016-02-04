#include "swatch/mp7/Utilities.hpp"

// MP7 Headers
#include "mp7/Orbit.hpp"

namespace swatch {
namespace mp7 {

namespace orbit {
// --------------------------------------------------------
bool
isValid( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric ) {
  return not (
        (aBx.isNaN() or aBx.value_ >= aMetric.bunchCount()) or
        (aCycle.isNaN() or aCycle.value_ >= aMetric.clockRatio())
      );

}


// --------------------------------------------------------
bool
isGood( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric ) {
  return not (aBx.isNaN() or aBx.value_ >= aMetric.bunchCount()) or
         (aCycle.isNaN() or aCycle.value_ >= aMetric.clockRatio());
}


// --------------------------------------------------------
bool
isNull( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle ) {
  return ( aBx.isNaN() and aCycle.isNaN() );
}

} // namespace orbit


} // namespace mp7
} // namespace swatch
