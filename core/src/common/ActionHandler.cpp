/**
 * @file    ActionHandler.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include "swatch/core/ActionHandler.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers

namespace swatch {
namespace core {


//---
ActionHandler::ActionHandler() : commands_() {
  // Make sure the map is correctly initialised
  commands_.clear();
}

//---
ActionHandler::~ActionHandler() {

  // Delete commands first
  BOOST_FOREACH( CommandMap::value_type p, commands_) {
    delete p.second;
  }
  // then Operations
  BOOST_FOREACH( OperationMap::value_type p, operations_) {
    delete p.second;
  }
  
  // Tidy things up
  commands_.clear();
  operations_.clear();
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

Operation*
ActionHandler::getOperation(const std::string& aName) {

  Operation* op = 0x0;
  try {
    op = operations_.at( aName );
  } catch ( const std::out_of_range& e ) {
    // Rethrow?
  }
  return op;
}

//---
std::set<std::string>
ActionHandler::getCommands() const {
  std::set<std::string> names;

 std::transform(commands_.begin(), commands_.end(), std::inserter(names, names.end()), boost::bind(&CommandMap::value_type::first, _1));
 return names;
}

//---
std::set<std::string>
ActionHandler::getOperations() const {
  std::set<std::string> names;

 std::transform(operations_.begin(), operations_.end(), std::inserter(names, names.end()), boost::bind(&OperationMap::value_type::first, _1));
 return names;
}

} // namespace core
} // namespace swatch
