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
  class BusyGuard;
public:
    
  class State : public AbstractState {
  public:
      State();
      
      const SystemStateMachine* getEngagedFSM() const;
      
  private:
    const SystemStateMachine* mFSM;
      
    // Friendship for classes/methods that will need to change system's state
    friend class ActionableSystem;
    friend class BusyGuard;
    friend class SystemStateMachine;
  };
    
  ActionableSystem(const std::string& aId);

  virtual ~ActionableSystem();

  /**
   * Names of stored state machines.
   * @return set of state machine names
   */
  std::set<std::string> getStateMachines() const;

  //! Get registered state machine of specified ID
  SystemStateMachine& getStateMachine( const std::string& aId );

  State getState() const;
  
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
  State mState;
  
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

