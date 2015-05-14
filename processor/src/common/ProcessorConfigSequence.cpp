#include "swatch/processor/ProcessorConfigSequence.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

namespace swatch {
namespace processor {

ProcessorConfigSequence::ProcessorConfigSequence( const std::string& aId ) :
  ConfigSequence( aId )
{
  
}

ProcessorConfigSequence::~ProcessorConfigSequence() {
}

void ProcessorConfigSequence::setTables()
{
  mTables = new std::deque< std::string >();

  core::Object* lParent = core::Object::getAncestor(1);
  if ( lParent )
  {
    mTables->push_back( lParent->path() ); //Config based on component ID has highest priority
    
    core::Object* lGrandparent = core::Object::getAncestor(2);
    if ( lGrandparent ) mTables->push_back( lGrandparent->path() + ".processors" ); //Then config based on system ID

    mTables->push_back( lParent->pset().get<ProcessorBag>("stub").bag.hwtype ); //Config based on hardware type has lowest priority
  }
}

} /* namespace core */
} /* namespace swatch */
