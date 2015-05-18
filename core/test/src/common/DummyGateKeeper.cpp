
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/core/ConfigSequence.hpp"

#include <sstream>

#include <xdata/Integer.h>
#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {

  DummyGateKeeper::DummyGateKeeper( Object* aToplevel ):
    core::GateKeeper( aToplevel , 0 )
  {}

  DummyGateKeeper::~DummyGateKeeper()
  {
  }
  
  xdata::Table* DummyGateKeeper::getTable( const std::string& aId )
  {
    xdata::Table* lTable = new xdata::Table();

    for ( uint32_t i(1); i<=2; ++i ){
      for ( uint32_t j(1); j<=6; ++j ){
        std::stringstream lStr, lStr2;
        xdata::Integer lXint( (6*i)+j );
        lStr  << "sequence_" << i << ".dummy_" << j << ".aa";
        lTable->addColumn( lStr.str() , "int" );  
        lTable->setValueAt( uint32_t(0) , lStr.str() , lXint );

        xdata::String lXstr( "print" );
        lStr2 << "sequence_" << i << ".dummy_" << j << ".todo";
        lTable->addColumn( lStr2.str() , "string" );
        lTable->setValueAt( uint32_t(0) , lStr2.str() , lXstr );
      }
    }

    std::cout << "DummyGateKeeper create xdata::Table called '" << aId << "' with content:\n" << lTable->toString() << std::endl;
    return lTable;
  }

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
