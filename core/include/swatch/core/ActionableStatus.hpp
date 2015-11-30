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
#include "boost/thread/lock_guard.hpp"
#include "boost/thread/mutex.hpp"

// SWATCH headers
#include "swatch/core/exception.hpp"


namespace swatch {
namespace core {

class ActionableSystem;
class Functionoid;
class MutableActionableStatus;


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

private:

  //! Indicates whether or not actions are allowed on this resource anymore (actions become disabled once the deleter is)
  bool mAlive;
  
  //! 
  std::string mStateMachineId;
  
  //!
  std::string mState;
  
  //! Indicates which functionoids (Command/CommandSequence/(System)Transition) are currently active; NULL value indicates that no functionoids are currently active.
  std::vector<const Functionoid*> mRunningActions;
  
  friend class MutableActionableStatus;
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


class ActionableStatusGuard {
public:
  explicit ActionableStatusGuard(const MutableActionableStatus& aStatus);
  ActionableStatusGuard(const MutableActionableStatus& aStatus, boost::adopt_lock_t);
  ~ActionableStatusGuard();
  
  const MutableActionableStatus& getStatus() const;
  
private:
  ActionableStatusGuard(); // no default CTOR
  ActionableStatusGuard(const ActionableStatusGuard&); // non-copyable
  ActionableStatus& operator=(const ActionableStatusGuard& aGuard);// non-assignable

  const MutableActionableStatus& mStatus;
  boost::lock_guard<boost::mutex> mLockGuard;
};


DEFINE_SWATCH_EXCEPTION(IncorrectActionableGuard);


class MutableActionableStatus {
public:
  MutableActionableStatus();
  ~MutableActionableStatus();

  ActionableStatus getSnapshot(const ActionableStatusGuard& aGuard) const;

  bool isAlive(const ActionableStatusGuard& aGuard) const;
  
  bool isEngaged(const ActionableStatusGuard& aGuard) const;

  const std::string& getStateMachineId(const ActionableStatusGuard& aGuard) const;
  
  const std::string& getState(const ActionableStatusGuard& aGuard) const;

  bool isBusy(const ActionableStatusGuard& aGuard) const;
  
  const std::vector<const Functionoid*>& getRunningActions(const ActionableStatusGuard& aGuard) const;

  const Functionoid* getLastRunningAction(const ActionableStatusGuard& aGuard) const;
  
  template<class T>
  const T* getFirstRunningActionOfType(const ActionableStatusGuard& aGuard) const;

  void setStateMachine(const std::string& aStateMachine, const std::string& aState, const ActionableStatusGuard& aGuard);

  void setNoStateMachine(const ActionableStatusGuard& aGuard);
  
  void setState(const std::string& aState, const ActionableStatusGuard& aGuard);
  
  void addAction(const Functionoid& aAction, const ActionableStatusGuard& aGuard);

  void popAction(const ActionableStatusGuard& aGuard);  

  //! Disables all future actions from running on this resource
  void kill(const ActionableStatusGuard& aGuard);
  
private:

  //! Throws if guard is not for this status instance
  void throwIfWrongGuard(const ActionableStatusGuard& aGuard) const;

  ActionableStatus mStatus;
  mutable boost::mutex mMutex;
  
  friend class ActionableStatusGuard;
  friend class ActionableSystem; // Temporary fix during refactoring - for ActionableSystem::lockMutexes
};


template<class T>
const T* MutableActionableStatus::getFirstRunningActionOfType(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.getFirstRunningActionOfType<T>();
}


} //end ns: core
} //end ns: swatch


#endif	/* __SWATCH_CORE_ACTIONABLESTATUS_HPP__ */

