#include "swatch/core/ActionHandler.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers

namespace swatch {
namespace core {


//---
ActionHandler::ActionHandler() {
}

//---
ActionHandler::~ActionHandler() {

  // Delete commands first
  BOOST_FOREACH( CommandMap::value_type p, commands_) {
    delete p.second;
//    LOG(swlo::kInfo) << "Command " << p.first << " deleted";
  }
  
  // Tidy things up
  commands_.clear();
}


//---
void
ActionHandler::registerCommand(std::string aName, Command* aCmd) {

  // Check if aCmd has already been registered
  if ( commands_.count(aName) ) {
    // Make it an exception, because the command already exists
    return;
  }
  
  commands_.emplace(aName, aCmd);
    
}


//---
Command*
ActionHandler::getCommand(const std::string& aName) {

  Command* cmd = 0x0;
  try {
    cmd = commands_.at( aName );
  } catch ( const std::out_of_range& e ) {
    // Rethrow?
  }
  return cmd;
}


} // namespace core
} // namespace swatch
