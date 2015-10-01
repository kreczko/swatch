/* 
 * File:   ActionableSystem.hpp
 * Author: tom
 *
 * Created on 18 September 2015, 15:02
 */

#ifndef __SWATCH_CORE_ACTIONABLESYSTEM_HPP__
#define	__SWATCH_CORE_ACTIONABLESYSTEM_HPP__


#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace core {

class SystemStateMachine;
class SystemTransition;
class Functionoid;

class ActionableSystem : public MonitorableObject {
public:
  ActionableSystem(const std::string& aId);

  virtual ~ActionableSystem();

  /**
   * Names of stored state machines.
   * @return set of state machine names
   */
  std::set<std::string> getStateMachines() const;

  //! Get registered state machine of specified ID
  SystemStateMachine& getStateMachine( const std::string& aId );

  //! Returns the currently running (or scheduled) action (i.e. commands, command sequences, transitions); returns NULL if no action is currently scheduled/running
  const Functionoid* getActiveFunctionoid() const;
  
  std::pair<const SystemStateMachine*, std::string> getState() const;
  
  void engageStateMachine(const std::string& aStateMachine);

  typedef boost::unordered_map< std::string , SystemStateMachine* > tStateMachineMap;

  //! Deleter functor that only deletes the actionable system after all system-level actions have finished running
  class Deleter : public Object::Deleter {
  public:
    Deleter() {}
    ~Deleter() {}
    
    void operator()(Object* aObject);
  };
  
protected:
  //! Register the supplied state machine in this object, with specified ID; this object takes ownership of the state machine.
  SystemStateMachine& registerStateMachine(const std::string& aId, const std::string& aInitialState, const std::string& aErrorState );    

  virtual void retrieveMetricValues() {}
  
private:

  //! Disables all future actions from running on this resource
  void disableActions();

  //! Locks mutex of system & all children involved in system state machine
  typedef boost::shared_ptr<boost::unique_lock<boost::mutex> > tLockGuardPtr;
  typedef  std::map<const swatch::core::MonitorableObject*, tLockGuardPtr> tLockGuardMap;
  static tLockGuardMap lockMutexes(const SystemStateMachine&);
  
  tStateMachineMap mFSMs;
  
  mutable boost::mutex mMutex;
  //! state machine that's currently active; equals NULL if no state machines are currently engaged
  const SystemStateMachine* mEngagedFSM;
  //! Current state in engaged state machine
  std::string mOpState;
  //! Indicates which functionoid is currently being executed; equals NULL if no functionoids are currently running
  const Functionoid* mActiveFunctionoid;
  //FIXME: Must fix any logic that checks if system is running transition
  
  //! Indicates whether of not actions are allowed on this resource anymore (actions become disabled once the deleter has been called)
  bool mEnabled;
  
  class BusyGuard {
  public:
    BusyGuard(ActionableSystem& aResource, const Functionoid& aAction);
    
    ~BusyGuard();
    
    const ActionableObject::BusyGuard& getChildGuard(const ActionableObject& aChild) const;
    
  private:
    ActionableSystem& mResource;
    const Functionoid& mAction;
    typedef boost::shared_ptr<const ActionableObject::BusyGuard> tChildGuardPtr;
    std::map<const ActionableObject*, tChildGuardPtr> mChildGuardMap;

    BusyGuard(const BusyGuard&); // non-copyable
    BusyGuard& operator=(const BusyGuard&); // non-assignable
  };

  friend class SystemStateMachine;
  friend class SystemTransition;
};


DEFINE_SWATCH_EXCEPTION(ActionableSystemIsBusy);


}
}

#endif	/* __SWATCH_CORE_ACTIONABLESYSTEM_HPP__ */

