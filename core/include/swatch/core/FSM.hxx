/* 
 * File:   ParameterSet.hxx
 * Author: ale
 *
 * Created on August 1, 2014, 12:30 PM
 */

#ifndef __SWATCH_CORE_FSM_HXX__
#define	__SWATCH_CORE_FSM_HXX__
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {

template<class OBJECT> void FSM::addStateTransition(const State& from, const State& to, const Input& input, OBJECT * obj,
            void (OBJECT::*func)(toolbox::Event::Reference)) {
      if (!hasState(from))
            throw exception("'from' State " + from + " is undeclared");
        if (!hasState(to))
            throw exception("'to' State " + to + " is undeclared");
        const XDAQState xdaq_from = getXDAQState(from);
        const XDAQState xdaq_to = getXDAQState(to);
        fsm_.addStateTransition<OBJECT>(xdaq_from, xdaq_to, input, obj, func);
    }

} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_FSM_HXX__ */
