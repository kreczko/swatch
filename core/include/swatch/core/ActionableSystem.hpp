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

//log4cplus headers
#include <log4cplus/logger.h>


namespace swatch {
namespace core {

class ActionableSystem;


class SystemFunctionoid : public Functionoid {
public:
  virtual ~SystemFunctionoid() { }
  
  const ActionableSystem& getActionable() const;

  ActionableSystem& getActionable();

  const std::set<ActionableObject*>& getParticipants();
  
protected:
  SystemFunctionoid(const std::string& aId, ActionableSystem& aActionable);
  
  //! Register a child of the system as participating in this functionoid (if child is already in participants list, method completes successfully but has no effect)
  void addParticipant(ActionableObject& aObj);

private:
  ActionableSystem& mActionable;
  std::set<ActionableObject*> mParticipants;
};


class ActionableSystem : public MonitorableObject {
public:
  typedef ActionableSnapshot Status_t;

  class StatusContainer : boost::noncopyable {
  public:
    StatusContainer(const ActionableSystem& aSystem, ActionableStatus& aSysStatus);
    ~StatusContainer();

    const ActionableStatus& getSystemStatus() const;
    const ActionableStatus& getStatus(const ActionableObject& aChild ) const;
  
    ActionableStatus& getSystemStatus();
    ActionableStatus& getStatus(const ActionableObject& aChild );

    typedef std::map<const MonitorableObject*, ActionableStatus*>::const_iterator iterator;

    iterator begin();
    iterator end();

  private:
    ActionableStatus& mSysStatus;
    std::map<const MonitorableObject*, ActionableStatus*> mStatusMap;
    
    friend class ActionableSystem;
  };

  ActionableSystem(const std::string& aId, const std::string& aLoggerName);

  virtual ~ActionableSystem();

  /*!
   * Names of stored state machines.
   * @return set of state machine names
   */
  std::set<std::string> getStateMachines() const;

  //! Get registered state machine of specified ID
  SystemStateMachine& getStateMachine( const std::string& aId );


  Status_t getStatus() const;

  log4cplus::Logger& getLogger();

  typedef boost::unordered_map< std::string , SystemStateMachine* > StateMachineMap_t;
  typedef boost::unordered_map< std::string , ActionableObject* > ActionableChildMap_t;

  //! Get registered actionable children
  const ActionableChildMap_t& getActionableChildren();

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
  
  void addActionable(ActionableObject* aChildObject);
  
private:

  StateMachineMap_t mFSMs;
  ActionableChildMap_t mActionableChildren;

  ActionableStatus mStatus;
  StatusContainer mStatusMap;
  log4cplus::Logger mLogger;
};


class SystemBusyGuard : public boost::noncopyable {
public:
  typedef boost::function<void(const ActionableStatusGuard&)> Callback_t;

  SystemBusyGuard(SystemFunctionoid& aAction, ActionableSystem::StatusContainer& aStatusMap, const ActionableStatusGuardMap_t& aStatusGuardMap, const Callback_t& aCallback);
  ~SystemBusyGuard();

  const BusyGuard& getChildGuard(const ActionableObject& aChild) const;

private:
  ActionableSystem& mSystem;
  ActionableStatus& mSysStatus;
  const SystemFunctionoid& mAction;
  const Callback_t mPostActionCallback;
  typedef boost::shared_ptr<const BusyGuard> ChildGuardPtr_t;
  std::map<const ActionableObject*, ChildGuardPtr_t> mChildGuardMap;
};


DEFINE_SWATCH_EXCEPTION(ActionableSystemIsBusy);


}
}

#endif	/* __SWATCH_CORE_ACTIONABLESYSTEM_HPP__ */

