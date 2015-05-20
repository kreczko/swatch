#include "swatch/processor/ProcessorCommandSequence.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

namespace swatch {
namespace processor {

ProcessorCommandSequence::ProcessorCommandSequence( const std::string& aId ) :
  CommandSequence( aId )
{
  
}

ProcessorCommandSequence::~ProcessorCommandSequence() {
}

std::vector< std::string >* ProcessorCommandSequence::setTables()
{
  std::vector< std::string >* lTables = new std::vector< std::string >();

  core::Object* lParent = getParent();
  if ( lParent )
  {
    lTables->push_back( lParent->path() ); //Config based on component ID has highest priority
    
    core::Object* lGrandparent = getParent(2);
    if ( lGrandparent ) lTables->push_back( lGrandparent->path() + ".processors" ); //Then config based on system ID

    lTables->push_back( lParent->pset().get<ProcessorBag>("stub").bag.hwtype ); //Config based on hardware type has lowest priority
  }

  return lTables;
}

} /* namespace core */
} /* namespace swatch */
