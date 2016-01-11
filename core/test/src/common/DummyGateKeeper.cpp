
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


void DummyGateKeeper::addTable(const std::string& aId, const ParametersTable_t& aTable)
{
  add(aId, aTable);
}

void DummyGateKeeper::addSettingsTable(const std::string& aId, const SettingsTable_t& aTable)
{
  add(aId, aTable);
}
  
  
void DummyGateKeeper::addMasksTable(const std::string& aId, const MasksTable_t& aTable)
{
  add(aId, aTable);
}
  
void DummyGateKeeper::addDisabledId(const std::string& aId)
{
  addToDisabledSet(aId);
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
