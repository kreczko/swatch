/**
 * @file    ActionableObject.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Actionables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_CORE_ActionableObject_HPP__
#define __SWATCH_CORE_ActionableObject_HPP__

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

class ActionableObject : public MonitorableObject {
public:
  explicit ActionableObject( const std::string& aId );

//  ActionableObject( const std::string& aId, const XParameterSet& aParams );
    
  virtual ~ActionableObject();

  /**
    * List of Configuration Sequence names stored.
    * @return set of command names
    */    
  std::set< std::string > getCommandSequences() const;

  /**
    * List of command names stored.
    * @return set of command names
    */
  std::set<std::string> getCommands() const;

  /**
   * List of operation names stored.
   * @return set of oepration names
   */
  std::set<std::string> getOperations() const;


  CommandSequence* getCommandSequence( const std::string& aId );
  Command* getCommand( const std::string& aId );
  Operation* getOperation( const std::string& aId );

  template< typename T>
  T* Register( const std::string& aId );

  void Register( const std::string& aId , CommandSequence* aCommandSequence );
  void Register(const std::string& aId , Command* aCommand );
  void Register(const std::string& aId , Operation* aOperation );


  typedef boost::unordered_map< std::string , CommandSequence* > tCommandSequenceMap;
  typedef boost::unordered_map< std::string , Command* > tCommandMap;
  typedef boost::unordered_map< std::string , Operation* > tOperationMap;
     
protected:

private:
  tCommandSequenceMap mCommandSequences;
  tCommandMap mCommands;
  tOperationMap mOperations;
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

#endif  /* __SWATCH_TEST_ActionableObject_HPP__ */

