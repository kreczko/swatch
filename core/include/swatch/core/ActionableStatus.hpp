/* 
 * File:   ActionableStatus.hpp
 * Author: tom
 *
 * Created on 16 November 2015, 14:08
 */

#ifndef __SWATCH_CORE_ACTIONABLESTATUS_HPP__
#define	__SWATCH_CORE_ACTIONABLESTATUS_HPP__


// STL headers
#include <string>
#include <vector>

// boost headers
#include "boost/static_assert.hpp"
#include "boost/type_traits/is_base_of.hpp"


namespace swatch {
namespace core {

class Functionoid;

//! Represents the current aggregate state (engaged FSM, FSM state, running actions, ...) of an ActionableObject/ActionableSystem
class ActionableStatus {
public:
    
  ActionableStatus();

  /**
   * The object is alive i.e. it is not bound for destruction
   * @return True if alive
   */
  bool isAlive() const;
  
  /**
   * The object is engaged with a State Machine
   * @return True of engaged
   */
  bool isEngaged() const;
  
  /**
   * One or more actions are currently running
   * @return True if one or more actions/functionoid are running
   */
  bool isRunning() const;
  
  //!
  const std::string& getState() const;
  
  //!
  const std::vector<const Functionoid*>& getRunningActions() const;
  
   
  const Functionoid* getLastRunningAction() const;
  
  //!
  template<class T>
  const T* getFirstRunningActionOfType() const;
  
  //!
  const std::string& getStateMachineId() const;
  
  //!
  static const std::string kNullStateMachineId;
  
  //!
  static const std::string kNullStateId;

protected:

  //! Indicates whether or not actions are allowed on this resource anymore (actions become disabled once the deleter is)
  bool mAlive;
  
  //! 
  std::string mStateMachineId;
  
  //!
  std::string mState;
  
  //! Indicates which functionoids (Command/CommandSequence/(System)Transition) are currently active; NULL value indicates that no functionoids are currently active.
  std::vector<const Functionoid*> mRunningActions;

  friend class ActionableObject;
  friend class ActionableSystem;
  friend class StateMachine;
  friend class SystemStateMachine;
  friend class BusyGuard;
};


template<class T>
const T* ActionableStatus::getFirstRunningActionOfType() const
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Functionoid,T>::value) );

  
  for(std::vector<const Functionoid*>::const_iterator lIt=mRunningActions.begin(); lIt!=mRunningActions.end(); lIt++)
  {
    if (const T* lAction = dynamic_cast<const T*>(*lIt) )
      return lAction;
  }
  
  return NULL;
}


} //end ns: core
} //end ns: swatch


#endif	/* __SWATCH_CORE_ACTIONABLESTATUS_HPP__ */

