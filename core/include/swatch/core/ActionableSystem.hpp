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

class SystemStateMachine;
class SystemTransition;
class Functionoid;


class ActionableSystem : public MonitorableObject {
  class BusyGuard;
public:
  typedef ActionableStatus Status_t;
  typedef MutableActionableStatus MutableStatus_t;
    
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

  typedef boost::unordered_map< std::string , SystemStateMachine* > tStateMachineMap;

  //! Deleter functor that only deletes the actionable system after all system-level actions have finished running
  class Deleter : public Object::Deleter {
  public:
    Deleter() {}
    ~Deleter() {}
    
    void operator()(Object* aObject);
  };
  
  //! Locks mutex of system & all children involved in system state machine
  typedef boost::shared_ptr<const ActionableStatusGuard> StatusGuardPtr_t;
  typedef std::map<const swatch::core::MonitorableObject*, StatusGuardPtr_t> StatusGuardMap_t;
  // TODO: move into being private state method of ActionableStatusGuard (or just method in same file), with collection of status pointers as argument
  static StatusGuardMap_t lockMutexes(const SystemStateMachine&);

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
  
  tStateMachineMap mFSMs;
  
  MutableStatus_t mStatus;  
  log4cplus::Logger mLogger;
  
  class BusyGuard : public boost::noncopyable {
  public:
    BusyGuard(ActionableSystem& aResource, const Functionoid& aAction);
    
    ~BusyGuard();
    
    const swatch::core::BusyGuard& getChildGuard(const ActionableObject& aChild) const;
    
  private:
    ActionableSystem& mSystem;
    MutableActionableStatus& mStatus;
    const Functionoid& mAction;
    typedef boost::shared_ptr<const swatch::core::BusyGuard> tChildGuardPtr;
    std::map<const ActionableObject*, tChildGuardPtr> mChildGuardMap;
  };

  friend class SystemTransition;
};


DEFINE_SWATCH_EXCEPTION(ActionableSystemIsBusy);


}
}

#endif	/* __SWATCH_CORE_ACTIONABLESYSTEM_HPP__ */

