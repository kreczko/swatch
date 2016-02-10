/* 
 * File:   AbstractForceStateCommand.hpp
 * Author: tom
 * Date:   February 2016
 */

#ifndef _SWATCH_DUMMY_ABSTRACTFORCESTATECOMMAND_HPP_
#define	_SWATCH_DUMMY_ABSTRACTFORCESTATECOMMAND_HPP_


#include "swatch/core/Command.hpp"
#include "swatch/dummy/ComponentState.hpp"


namespace swatch {
namespace dummy {


class AbstractForceStateCommand : public core::Command {
public:
  AbstractForceStateCommand(const std::string& aId, core::ActionableObject& aActionable);
  virtual ~AbstractForceStateCommand();

protected:
  static ComponentState parseState(const core::XParameterSet& aParameSet);  
};


} // end ns: dummy
} // end ns: swatch

#endif	/* _SWATCH_DUMMY_ABSTRACTFORCESTATECOMMAND_HPP_ */

