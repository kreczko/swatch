
#include "swatch/core/test/DummyGateKeeper.hpp"


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


void DummyGateKeeper::addTable(const std::string& aId, const tTable& aTable)
{
  add(aId, aTable);
}

void DummyGateKeeper::addSettingsTable(const std::string& aId, const tSettingsTable& aTable)
{
  add(aId, aTable);
}
  
  
void DummyGateKeeper::addMasksTable(const std::string& aId, const MasksTable_t& aTable)
{
  add(aId, aTable);
}
  
  

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
