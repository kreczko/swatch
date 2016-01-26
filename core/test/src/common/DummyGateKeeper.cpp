
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


void DummyGateKeeper::addContext(const std::string& aId, const ParametersContext_t& aContext)
{
  add(aId, aContext);
}

void DummyGateKeeper::addSettingsContext(const std::string& aId, const SettingsContext_t& aContext)
{
  add(aId, aContext);
}
  
  
void DummyGateKeeper::addMasksContext(const std::string& aId, const MasksContext_t& aContext)
{
  add(aId, aContext);
}
  
void DummyGateKeeper::addDisabledId(const std::string& aId)
{
  addToDisabledSet(aId);
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
