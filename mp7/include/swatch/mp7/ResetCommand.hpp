
#ifndef __SWATCH_MP7_MP7RESETCOMMAND_HPP__
#define __SWATCH_MP7_MP7RESETCOMMAND_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace mp7 {

/**
 * @class ResetCommand
 * Issues a global reset to the MP7 and configures clocking and ttc infrastructure
 */  
class ResetCommand : public core::Command
{
public:
  ResetCommand(const std::string& aId, core::ActionableObject& aActionable);
  virtual ~ResetCommand() {}
  virtual State code(const core::XParameterSet& params);

};

class SetIDCommand : public core::Command
{
public:
    SetIDCommand(const std::string& aId, core::ActionableObject& aActionable);
    ~SetIDCommand() {}
    virtual State code(const core::XParameterSet& params);
};



} //end ns mp7
} //end ns swatch

#endif /* __SWATCH_MP7_MP7RESETCOMMAND_HPP__ */
