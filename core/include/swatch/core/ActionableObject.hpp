/**
 * @file    ActionableObject.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Actionables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_TEST_ActionableObject_HPP__
#define __SWATCH_TEST_ActionableObject_HPP__

// SWATCH Headers
#include "swatch/core/exception.hpp"
#include "swatch/core/MonitorableObject.hpp"
#include "swatch/core/ConfigSequence.hpp"
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

  ActionableObject( const std::string& aId, const XParameterSet& aParams );
    
  virtual ~ActionableObject();

  /**
    * List of Configuration Sequence names stored.
    * @return set of command names
    */    
  std::set< std::string > getConfigSequences() const;

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


  ConfigSequence* getConfigSequence( const std::string& aId );
  Command* getCommand( const std::string& aId );
  Operation* getOperation( const std::string& aId );

  template< typename T>
  T* Register( const std::string& aId );

  void Register( const std::string& aId , ConfigSequence* aConfigSequence );
  void Register(const std::string& aId , Command* aCommand );
  void Register(const std::string& aId , Operation* aOperation );


  typedef boost::unordered_map< std::string , ConfigSequence* > tConfigSequenceMap;
  typedef boost::unordered_map< std::string , Command* > tCommandMap;
  typedef boost::unordered_map< std::string , Operation* > tOperationMap;
     
protected:

private:
  tConfigSequenceMap mConfigSequences;
  tCommandMap mCommands;
  tOperationMap mOperations;
};

DEFINE_SWATCH_EXCEPTION(ConfigSequenceAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandAlreadyExistsInActionableObject);
DEFINE_SWATCH_EXCEPTION(OperationAlreadyExistsInActionableObject);

DEFINE_SWATCH_EXCEPTION(ConfigSequenceNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(CommandNotFoundInActionableObject);
DEFINE_SWATCH_EXCEPTION(OperationNotFoundInActionableObject);

} // namespace core
} // namespace swatch

#include "swatch/core/ActionableObject.hxx"

#endif  /* __SWATCH_TEST_ActionableObject_HPP__ */

