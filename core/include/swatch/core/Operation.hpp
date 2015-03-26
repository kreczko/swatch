/*
 * Operation.h
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 *
 * The Operation (dev name) class is used as a mediator for classes that inherit from swatch::processor::Processor.
 * It provides a Finite State Machine (swatch::core::FSM) to cycle through pre-defined
 * (in an implementation of Operation) states and call Operation methods that are associated transitions.
 * The Operation methods then use the Handler resource to manipulate the hardware underneath.
 */

#ifndef __SWATCH_CORE_OPERATION_HPP__
#define __SWATCH_CORE_OPERATION_HPP__
// C++ Headers
#include <string>

// boost headers
#include <boost/unordered_map.hpp>

// Swatch Headers
#include "swatch/core/FSM.hpp"
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace core {
class ActionHandler;

typedef ActionHandler Resource;

class Operation {
public:

  enum Status {
    kInitial, kRunning, kError, kWarning, kDone,
  };
  virtual ~Operation();

  template<typename R> R* getResource();

  // OLD
  /*
   * Returns if preconditions of the operation are met.
   * Currently this only involves a check if the FSM
   * can make the state transition
   */
  virtual bool preCondition() const;

//  void setStateTransition(StateTransition t);

  void executeTransition(const std::string);
  std::string getCurrentState() const;

  XParameterSet & getParams();

  const std::vector<std::string>& getStates() const;
  
  const std::map<std::string, std::string> getStateTransitions(const std::string& s) const;

  float getProgress() const;
  
  std::string progressMsg() const;

  std::string getStatus() const;

protected:
  Operation(ActionHandler* resource);

  XParameterSet parameters_;
  FSM* fsm_;
//  template<typename T>
//  Operation( Resource* resource, const T& aDefault, FSM* fsm );

private:
  ActionHandler* resource_;

//  StateTransition transition_;

};
typedef boost::unordered_map<std::string, Operation*> OperationMap;
} /* namespace core */
} /* namespace swatch */

#include "Operation.hxx"

#endif /* __SWATCH_CORE_OPERATION_HPP__ */
