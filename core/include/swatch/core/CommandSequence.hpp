/**
 * @file    CommandSequence.hpp
 * @author  arose
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_COMMANDSEQUENCE_HPP__
#define __SWATCH_CORE_COMMANDSEQUENCE_HPP__


#include "swatch/core/CommandVec.hpp"


namespace swatch {
namespace core {

class Command;
class CommandStatus;
class GateKeeper;
class StateMachine;


//! Represents a sequence of commands, executed in succession.
class CommandSequence : public CommandVec {
public:
  CommandSequence( const std::string& aId, ActionableObject& aResource, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias="");
  CommandSequence( const std::string& aId, ActionableObject& aResource, Command& aFirstCommand, const std::string& aFirstCommandAlias="");

  virtual ~CommandSequence();
  
  /**
    Utility function to add a command to the command sequence
    @param aCommand a command to add to the command sequence
    @param aAlias an alias to use for that command when looking up parameters in gatekeeper; the command's ID string is used if alias is empty
  */
  CommandSequence& run( Command& aCommand, const std::string& aAlias="");
  CommandSequence& then( Command& aCommand, const std::string& aAlias="");

  CommandSequence& run( const std::string& aCommand, const std::string& aAlias="");
  CommandSequence& then( const std::string& aCommand, const std::string& aAlias="");
};


typedef CommandVecStatus CommandSequenceStatus;

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_COMMANDSEQUENCE_HPP__ */
