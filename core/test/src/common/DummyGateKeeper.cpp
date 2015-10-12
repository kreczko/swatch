
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/core/CommandSequence.hpp"

#include <sstream>

#include <xdata/Integer.h>
#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {

DummyGateKeeper::DummyGateKeeper():
  core::GateKeeper( "__dummyGK__" )
{
}

DummyGateKeeper::~DummyGateKeeper()
{
}


void DummyGateKeeper::addTable(const std::string& aId, tTable& aTable)
{
  add(aId, aTable);
}
  
  

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
