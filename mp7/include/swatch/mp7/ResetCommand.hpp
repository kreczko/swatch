
#ifndef __SWATCH_MP7_MP7RESETCOMMAND_HPP__
#define __SWATCH_MP7_MP7RESETCOMMAND_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace mp7 {

  
class ResetCommand : public ::swatch::core::Command {
public:
  ResetCommand(const std::string& aId);
  virtual ~ResetCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};


} //end ns mp7
} //end ns swatch

#endif /* __SWATCH_MP7_MP7RESETCOMMAND_HPP__ */
