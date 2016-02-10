/* 
 * File:   AbstractConfigureCommand.hpp
 * Author: tom
 * Date:   February 2016
 */

#ifndef _SWATCH_DUMMY_ABSTRACTCONFIGURECOMMAND_HPP_
#define	_SWATCH_DUMMY_ABSTRACTCONFIGURECOMMAND_HPP_


#include "swatch/core/Command.hpp"


namespace swatch {
namespace dummy {


class AbstractConfigureCommand : public core::Command {
public:
  AbstractConfigureCommand(const std::string& aId, core::ActionableObject& aActionable);
  virtual ~AbstractConfigureCommand();

protected:
  State code(const core::XParameterSet& aParams);
  
  virtual void runAction(bool aErrorOccurs) = 0;
};


} // end ns: dummy
} // end ns: swatch

#endif	/* _SWATCH_DUMMY_ABSTRACTCONFIGURECOMMAND_HPP_ */

