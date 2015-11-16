/**
 * @file    ActionableObject.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Actionables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_CORE_ACTIONABLEOBJECT_HPP__
#define __SWATCH_CORE_ACTIONABLEOBJECT_HPP__

// SWATCH Headers
#include "swatch/core/exception.hpp"
#include "swatch/core/MonitorableObject.hpp"

// STL Headers
#include <set>
#include <string>

// BOOST Headers
#include "boost/unordered_map.hpp"
#include "Functionoid.hpp"

namespace swatch {
namespace core {


class ActionableObject;
class ActionableSystem;
class Command;
class CommandSequence;
class StateMachine;
class SystemStateMachine;


class ActionableStatus {
public:
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
   * @return True if one or more actions/funtionoid are running
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

  ActionableStatus();

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

/**
 * @class ActionableFunctoid
 */
class ActionableFunctionoid : public Functionoid {
public:
  virtual ~ActionableFunctionoid() {

  }
  
  const ActionableObject& getActionable() const;

  ActionableObject& getActionable();

  template<typename T> const T& getActionable() const ;

  template<typename T> T& getActionable();
  
protected:

  ActionableFunctionoid(const std::string& aId, ActionableObject& aActionable );
  
};

/**
 * @class ActionableObject
 * @brief  An object representing a resource on which commands, command sequences, and transitions run
 */

class ActionableObject : public MonitorableObject {
  class BusyGuard;
public:
  
  typedef ActionableStatus Status;

  explicit ActionableObject( const std::string& aId );

  virtual ~ActionableObject();

  /**
    * Names of stored command sequences.
    * @return set of command sequence names
    */    
  std::set< std::string > getSequences() const;

  /**
    * Names of stored commands.
    * @return set of command names
    */
  std::set<std::string> getCommands() const;

  /**
   * Names of stored state machines.
   * @return set of state machine names
   */
  std::set<std::string> getStateMachines() const;

  //! Get registered command sequence of specified ID
  CommandSequence& getSequence( const std::string& aId );

  //! Get registered command of specified ID
  Command& getCommand( const std::string& aId );

  //! Get registered state machine of specified ID
  StateMachine& getStateMachine( const std::string& aId );

  virtual const std::vector<std::string>& getGateKeeperTables() const = 0;
  
  Status getStatus() const;
  
  //! Engage state machine of specified ID
  void engageStateMachine(const std::string& aStateMachine);
  
  typedef boost::unordered_map< std::string , CommandSequence* > tCommandSequenceMap;
  typedef boost::unordered_map< std::string , Command* > tCommandMap;
  typedef boost::unordered_map< std::string , StateMachine* > tStateMachineMap;

  //! Deleter functor that only deletes the actionable object after all commands, command sequences and transitions have finished running
  class Deleter : public Object::Deleter {
  public:
    Deleter() {}
    ~Deleter() {}
    
    void operator()(Object* aObject);
  };

protected:
  //! Register the supplied command class in this object, with specified ID; the class is constructed on the heap, using the ID as the only constructor argument.
  template< typename T>
  T& registerCommand( const std::string& aId );
  
  //! Register the supplied command in this object, with specified ID; this object takes ownership of the command sequence.
  Command& registerCommand(const std::string& aId , Command* aCommand );
  
  //! Register the a command sequence in this object, with specified ID
  CommandSequence& registerSequence(const std::string& aId, const std::string& aFirstCommandId, const std::string& aFirstCommandNamespace="");
  
  //! Register the a command sequence in this object, with specified ID
  CommandSequence& registerSequence(const std::string& aId, Command& aFirstCommand, const std::string& aFirstCommandNamespace="");


  /*!
   * @brief Register a finite state machine in this object, with specified ID
   * @param aId State machine's ID
   * @param aInitialState The FSM's initial state
   * @param aErrorState The FSM's error state
   */ 
  StateMachine& registerStateMachine(const std::string& aId, const std::string& aInitialState, const std::string& aErrorState );    
  
private:
  //! Disables all future actions from running on this resource
  void kill();

  tCommandSequenceMap mCommandSequences;
  tCommandMap mCommands;
  tStateMachineMap mFSMs;

  mutable boost::mutex mMutex;
  Status mStatus;

  class BusyGuard : public boost::noncopyable {
  public:
    BusyGuard(ActionableObject& aResource, const Functionoid&, const BusyGuard* aOuterGuard=NULL);
    BusyGuard(ActionableObject& aResource, const boost::unique_lock<boost::mutex>& aLockGuard, const Functionoid&, const BusyGuard* aOuterGuard=NULL);

    ~BusyGuard();
    
  private:
    ActionableObject& mResource;
    const Functionoid& mAction;
    const BusyGuard* mOuterGuard;
    
    void initialise(const boost::unique_lock<boost::mutex>& aLockGuard);
  };

public:
  struct ActionFmt {
  public:
    ActionFmt(const Functionoid* aAction);
    ~ActionFmt();
    const Functionoid* const mAction;
  };

  friend class ActionableSystem;
  friend class Command;
  friend class CommandSequence;
  friend class CommandVec;
  friend class StateMachine;
  friend class SystemTransition;
  friend class SystemStateMachine;
};


std::ostream& operator<<(std::ostream& aStream, const ActionableObject::ActionFmt& aActionFmt);


DEFINE_SWATCH_EXCEPTION(CommandSequenceAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(StateMachineAlreadyExistsInActionableObject);

DEFINE_SWATCH_EXCEPTION(CommandSequenceNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(StateMachineNotFoundInActionableObject);

DEFINE_SWATCH_EXCEPTION(ActionableObjectIsBusy);
DEFINE_SWATCH_EXCEPTION(ParameterNotFound);
DEFINE_SWATCH_EXCEPTION(ResourceInWrongState);
DEFINE_SWATCH_EXCEPTION(ResourceInWrongStateMachine);
DEFINE_SWATCH_EXCEPTION(WrongBusyGuard);


} // namespace core
} // namespace swatch

#include "swatch/core/ActionableObject.hxx"

#endif  /* __SWATCH_CORE_ACTIONABLEOBJECT_HPP__ */

