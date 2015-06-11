/*
 * Operation.h
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 *
 * The Operation (dev name) class is used as a mediator for classes that inherit from swatch::processor::Processor.
 */

#ifndef __SWATCH_CORE_OPERATION_HPP__
#define __SWATCH_CORE_OPERATION_HPP__
// C++ Headers
#include <string>

// boost headers
#include <boost/unordered_map.hpp>

// Swatch Headers
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/FSM.hpp"
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace core {
  class ActionableObject;
  class CommandSequence;  

/*
 * @brief Represents the multi-command operations (FSMs) of classes that inherit from swatch::core::ActionResource.
 * It provides a Finite State Machine (swatch::core::FSM) to cycle through pre-defined
 * (in an implementation of Operation) states and call Operation methods that are associated transitions.
 * The Operation methods then use the Handler resource to manipulate the hardware underneath.
 */

class Operation : public Functionoid {
public:

  virtual ~Operation();

  void executeTransition(const std::string);

  std::string getCurrentState() const;

  const std::vector<std::string>& getStates() const;
  
  const std::map<std::string, std::string> getStateTransitions(const std::string& s) const;

  float getProgress() const;
  
  std::string progressMsg() const;

  std::string getStatus() const;


  void addTransition(const std::string& from, const std::string& to, const std::string& event, CommandSequence& aCommandSequence );

  void addTransition(const std::string& from, const std::string& to, const std::string& event, const std::string& aCommandSequenceId );

protected:
  Operation( const std::string& aId );

  FSM* fsm_;

private:

};
typedef boost::unordered_map<std::string, Operation*> OperationMap;
} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_OPERATION_HPP__ */
