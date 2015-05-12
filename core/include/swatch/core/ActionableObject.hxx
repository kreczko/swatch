#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

template < typename T >
void ActionableObject::registerConfigSequence( const std::string& aId )
{
//   BOOST_STATIC_ASSERT( (boost::is_base_of<ConfigSequence,T>::value) );
  if (mConfigSequences.count(aId)) throw ConfigSequenceAlreadyExistsInActionableObject(aId);
  mConfigSequences.emplace( aId , new T(this) );
}

template< typename T>
void ActionableObject::registerCommand( const std::string& aId )
{
//   BOOST_STATIC_ASSERT( (boost::is_base_of<Command,T>::value) ); 
  if (mCommands.count(aId)) throw CommandAlreadyExistsInActionableObject(aId);
  mCommands.emplace( aId , new T(this) );
}

template< typename T >
void ActionableObject::registerOperation( const std::string& aId ) {
//   BOOST_STATIC_ASSERT( (boost::is_base_of<Operation,T>::value) ); 
  if (mOperations.count(aId)) throw OperationAlreadyExistsInActionableObject(aId);
  mOperations.emplace( aId , new T(this) );
}

}
}
