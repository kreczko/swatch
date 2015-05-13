#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

template < typename T >
ConfigSequence* ActionableObject::registerConfigSequence( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<ConfigSequence,T>::value) );
  if (mConfigSequences.count(aId)) throw ConfigSequenceAlreadyExistsInActionableObject(aId);
  ConfigSequence* lConfigSequence( new T( aId ) );
  this->addObj(lConfigSequence);
  mConfigSequences.insert( std::make_pair( aId , lConfigSequence ) );
  return lConfigSequence;
}

template< typename T>
Command* ActionableObject::registerCommand( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<Command,T>::value) ); 
  if (mCommands.count(aId)) throw CommandAlreadyExistsInActionableObject(aId);
  Command* lCommand( new T( aId ) );
  this->addObj(lCommand);
  mCommands.insert( std::make_pair( aId , lCommand ) );
  return lCommand;
}

template< typename T >
Operation* ActionableObject::registerOperation( const std::string& aId ) {
  BOOST_STATIC_ASSERT( (boost::is_base_of<Operation,T>::value) ); 
  if (mOperations.count(aId)) throw OperationAlreadyExistsInActionableObject(aId);
  Operation* lOperation( new T( aId ) );
  this->addObj(lOperation);
  mOperations.insert( std::make_pair( aId , lOperation ) );
  return lOperation;
}

}
}
