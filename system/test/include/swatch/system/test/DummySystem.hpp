/* 
 * File:   DummySystem.hpp
 * Author: tom
 *
 * Created on 18 September 2015, 14:05
 */

#ifndef __SWATCH_SYSTEM_DUMMYSYSTEM_HPP__
#define	__SWATCH_SYSTEM_DUMMYSYSTEM_HPP__


#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/system/System.hpp"


namespace swatch {
namespace system {
namespace test {

class DummySystem : public swatch::system::System {
public:
  DummySystem(const swatch::core::AbstractStub& aStub);
  ~DummySystem();

};

}
}
}


#endif	/* __SWATCH_SYSTEM_DUMMYSYSTEM_HPP__ */

