/* 
 * File:   Operation.hxx
 * Author: kreczko
 *
 * Created on 16 Feb 2015
 */

#ifndef __SWATCH_CORE_OPERATION_HXX__
#define	__SWATCH_CORE_OPERATION_HXX__
#include "swatch/core/Operation.hpp"
namespace swatch {
namespace core {
template<typename R>
R* Operation::getResource() {
  return dynamic_cast<R*>(resource_);
}
} // namespace core
} // namespace swatch
#endif	/* __SWATCH_CORE_OPERATION_HXX__ */
