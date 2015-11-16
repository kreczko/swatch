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
  typedef ActionableStatus State;
    
  ActionableSystem(const std::string& aId);

  virtual ~ActionableSystem();

  /*!
   * Names of stored state machines.
   * @return set of state machine names
   */
  std::set<std::string> getStateMachines() const;

  //! Get registered state machine of specified ID
  SystemStateMachine& getStateMachine( const std::string& aId );

  State getState() const;
  
  //! Engage state machine of specified ID
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
  /*!
   * @brief Register a finite state machine in this system, with specified ID
   * @param aId State machine's ID
   * @param aInitialState The FSM's initial state
   * @param aErrorState The FSM's error state
   */ 
  SystemStateMachine& registerStateMachine(const std::string& aId, const std::string& aInitialState, const std::string& aErrorState );    

  virtual void retrieveMetricValues() {}
  
private:

  //! Disables all future actions from running on this resource
  void disableActions();

  //! Locks mutex of system & all children involved in system state machine
  typedef boost::shared_ptr<boost::unique_lock<boost::mutex> > tLockGuardPtr;
  typedef std::map<const swatch::core::MonitorableObject*, tLockGuardPtr> tLockGuardMap;
  static tLockGuardMap lockMutexes(const SystemStateMachine&);
  
  tStateMachineMap mFSMs;
  
  mutable boost::mutex mMutex;
  State mStatus;
  
  class BusyGuard : public boost::noncopyable {
  public:
    BusyGuard(ActionableSystem& aResource, const Functionoid& aAction);
    
    ~BusyGuard();
    
    const ActionableObject::BusyGuard& getChildGuard(const ActionableObject& aChild) const;
    
  private:
    ActionableSystem& mResource;
    const Functionoid& mAction;
    typedef boost::shared_ptr<const ActionableObject::BusyGuard> tChildGuardPtr;
    std::map<const ActionableObject*, tChildGuardPtr> mChildGuardMap;
  };

  friend class SystemStateMachine;
  friend class SystemTransition;
};


DEFINE_SWATCH_EXCEPTION(ActionableSystemIsBusy);


}
}

#endif	/* __SWATCH_CORE_ACTIONABLESYSTEM_HPP__ */

