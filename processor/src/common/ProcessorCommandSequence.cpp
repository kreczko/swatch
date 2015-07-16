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

  // Parent is a processor
//  core::Object* lParent = getParent();
  swatch::processor::Processor* lParent = getParent<swatch::processor::Processor>();
  if ( lParent )
  {
    lTables->push_back( lParent->getPath() ); //Config based on component ID has highest priority
    

    /*
    core::Object* lGrandparent = getParent(2);
    if ( lGrandparent ) lTables->push_back( lGrandparent->path() + ".processors" ); //Then config based on system ID
     */
    
    //FIXME (Tom's fault - sorry, tag to release): Slightly hacky workaround since getParent(size_t ) method has been removed; there may be a cleaner implementation ...
    std::string lParentPath( getParent()->getPath() );
    if ( lParentPath.size() > getParent()->getId().size() ) {
      std::string lGrandparentPath( lParentPath.substr(0, lParentPath.size() - getParent()->getId().size() - 1) );
      lTables->push_back( lGrandparentPath + ".processors" );
    }

    // Alessandro TODO: understand what this is for
    // lTables->push_back( lParent->pset().get<ProcessorBag>("stub").bag.hwtype ); //Config based on hardware type has lowest priority
   lTables->push_back( lParent->getStub().hwtype ); //Config based on hardware type has lowest priority
  }

  return lTables;
}

} /* namespace core */
} /* namespace swatch */
