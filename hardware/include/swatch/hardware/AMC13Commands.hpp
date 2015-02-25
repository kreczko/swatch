/**
 * @file    AMC13Commands
 * @details [long description]
 * @author  Alessandro Thea
 * @data    February 2015
 * 
 */

#ifndef __SWATCH_HARDWARE_AMC13COMMANDS_HPP__
#define __SWATCH_HARDWARE_AMC13COMMANDS_HPP__

#include "swatch/core/Command.hpp"

namespace swatch {
namespace hardware {

class AMC13ResetCommand : public swatch::core::Command {
public:
    AMC13ResetCommand(core::ActionHandler* aHandler);
    virtual ~AMC13ResetCommand();
    

    virtual void exec();

    
};

} // namespace hardware
} // namespace swatch

#endif