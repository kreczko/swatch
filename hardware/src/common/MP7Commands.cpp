/* 
 * @file    MP7Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */



#include "swatch/hardware/MP7Commands.hpp"

// XDAQ Headers
#include "xdata/Integer.h"
#include "swatch/hardware/MP7Processor.hpp"
#include "mp7/MP7Controller.hpp"

namespace swatch {
namespace hardware {


//---
MP7ResetCommand::MP7ResetCommand(core::ActionHandler* aHandler) : 
    Command(aHandler, xdata::Integer() ) {

}

//---
MP7ResetCommand::~MP7ResetCommand() {

}

//---
void MP7ResetCommand::exec() {

  MP7Processor* p = getHandler<MP7Processor>();
  
  std::string mode = "external";
  p->driver().reset(mode, mode, mode);

}



} /* namespace hardware */
} /* namespace swatch */