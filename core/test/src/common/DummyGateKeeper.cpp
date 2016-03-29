
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

void DummyGateKeeper::addSettingsContext(const std::string& aContextId, const MonitoringSettings_t& aSettingsMap)
{
  add(aContextId, SettingsContext_t(new MonitoringSettings_t(aSettingsMap)));
}
  
  
void DummyGateKeeper::addMasksContext(const std::string& aContextId, const Masks_t& aMasksSet)
{
  add(aContextId, MasksContext_t(new Masks_t(aMasksSet)));
}

  
void DummyGateKeeper::addDisabledId(const std::string& aId)
{
  addToDisabledSet(aId);
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
