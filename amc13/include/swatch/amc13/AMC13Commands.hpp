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
    
class RebootCommand : public swatch::core::Command {
public:
    RebootCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    
    virtual ~RebootCommand();
    
    virtual State code(const core::XParameterSet& params);
};

class ResetCommand : public swatch::core::Command {
public:
    ResetCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual ~ResetCommand();

    virtual State code(const core::XParameterSet& params);
    
};

class ConfigureTTCCommand : public swatch::core::Command {
public:
    ConfigureTTCCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~ConfigureTTCCommand();
    
    virtual State code(const core::XParameterSet& params);

};

class ConfigureDAQCommand : public swatch::core::Command {
public:
    ConfigureDAQCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~ConfigureDAQCommand();
    
    virtual State code(const core::XParameterSet& params);

};

class StartCommand : public swatch::core::Command {
public:
    StartCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~StartCommand();

    virtual State code(const core::XParameterSet& params);

};

class StopCommand : public swatch::core::Command {
public:
    StopCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~StopCommand();
    
    virtual State code(const core::XParameterSet& params);

};


} // namespace amc13
} // namespace swatch

#endif
