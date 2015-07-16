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
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/Operation.hpp"

// STL Headers
#include <set>
#include <string>

// BOOST Headers
#include "boost/unordered_map.hpp"

namespace swatch {
namespace core {

//! An object representing a resource on which commands, operations and command sequences run
class ActionableObject : public MonitorableObject {
public:
  explicit ActionableObject( const std::string& aId );

  virtual ~ActionableObject();

  /**
    * Names of stored command sequences.
    * @return set of command sequence names
    */    
  std::set< std::string > getCommandSequences() const;

  /**
    * Names of stored commands.
    * @return set of command names
    */
  std::set<std::string> getCommands() const;

  /**
   * Names of stored operations.
   * @return set of operation names
   */
  std::set<std::string> getOperations() const;

  //! Get registered command sequence of specified ID
  CommandSequence& getCommandSequence( const std::string& aId );

  //! Get registered command of specified ID
  Command& getCommand( const std::string& aId );

  //! Get registered command of specified ID
  Operation& getOperation( const std::string& aId );

  //! Returns whether or not actions (i.e. commands, command sequences, operations) are currently enabled on this resource
  bool isEnabled() const;

  //! Returns the currently running (or scheduled) action (i.e. commands, command sequences, operations); returns NULL if no action is currently scheduled/running
  const Functionoid* getActiveFunctionoid() const;
  
  typedef boost::unordered_map< std::string , CommandSequence* > tCommandSequenceMap;
  typedef boost::unordered_map< std::string , Command* > tCommandMap;
  typedef boost::unordered_map< std::string , Operation* > tOperationMap;

  //! Deleter functor that only deletes the actionable object after all commands, command sequences and operations have finished running
  class Deleter : public Object::Deleter {
  public:
    Deleter() {}
    ~Deleter() {}
    
    void operator()(Object* aObject);
  };
  
protected:
  //! Register the supplied command, command sequence or operation class in this object, with specified ID; the class is constructed on the heap, using the ID as the only constructor argument.
  template< typename T>
  T& registerFunctionoid( const std::string& aId );

  //! Register the supplied command sequence in this object, with specified ID; this object takes ownership of the command sequence.
  CommandSequence& registerFunctionoid(const std::string& aId , CommandSequence* aCommandSequence );
  //! Register the supplied command in this object, with specified ID; this object takes ownership of the command sequence.
  Command& registerFunctionoid(const std::string& aId , Command* aCommand );
  //! Register the supplied operation in this object, with specified ID; this object takes ownership of the command sequence.
  Operation& registerFunctionoid(const std::string& aId , Operation* aOperation );    
  
private:
  //! Disables all future actions from running on this resource
  void disableActions();

  tCommandSequenceMap mCommandSequences;
  tCommandMap mCommands;
  tOperationMap mOperations;
  
  mutable boost::mutex mMutex;
  //! Indicates which functionoid (Command/CommandSequence/Operation) is currently active; NULL value indicates that no functionoids are currently active.
  const Functionoid* mActiveFunctionoid;
  //! Indicates whether or not actions are allowed on this resource anymore (actions become disabled once the deleter is )
  bool mEnabled;

  std::pair<bool, Functionoid const * > requestControlOfResource(Functionoid const * const aFunctionoid);
  Functionoid const * releaseControlOfResource(Functionoid const * const aFunctionoid);
  
  friend class Command;
  friend class CommandSequence;
  friend class Operation;
};

DEFINE_SWATCH_EXCEPTION(CommandSequenceAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(OperationAlreadyExistsInActionableObject);

DEFINE_SWATCH_EXCEPTION(CommandSequenceNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(OperationNotFoundInActionableObject);

} // namespace core
} // namespace swatch

#include "swatch/core/ActionableObject.hxx"

#endif  /* __SWATCH_CORE_ACTIONABLEOBJECT_HPP__ */

