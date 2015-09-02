/**
 * @file    AMC13Commands
 * @details [long description]
 * @author  Alessandro Thea
 * @data    February 2015
 * 
 */

#ifndef __SWATCH_AMC13_AMC13COMMANDS_HPP__
#define __SWATCH_AMC13_AMC13COMMANDS_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace amc13 {
    
class AMC13RebootCommand : public swatch::core::Command {
public:
    AMC13RebootCommand(const std::string& aId);
    
    virtual ~AMC13RebootCommand();
    
    virtual State code(const core::XParameterSet& params);
};

class AMC13ResetCommand : public swatch::core::Command {
public:
    AMC13ResetCommand(const std::string& aId);

    virtual ~AMC13ResetCommand();

    virtual State code(const core::XParameterSet& params);
    
};

} // namespace amc13
} // namespace swatch

#endif
