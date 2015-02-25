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

//---
std::set<std::string>
ActionHandler::getCommands() const {
  std::set<std::string> names;

 std::transform(commands_.begin(), commands_.end(), std::inserter(names, names.end()), boost::bind(&CommandMap::value_type::first, _1));
 return names;
}

} // namespace core
} // namespace swatch
