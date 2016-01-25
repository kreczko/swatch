/**
 * @file    DummyGateKeeper.hpp
 * @author  arose
 * @brief   Emulate a GateKeeper
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_DummyGateKeeper_HPP__
#define __SWATCH_CORE_DummyGateKeeper_HPP__

// Swatch Headers
#include "swatch/core/GateKeeper.hpp"


namespace swatch {
namespace core {
namespace test {

class DummyGateKeeper : public GateKeeper {
  public:

  DummyGateKeeper();

  virtual ~DummyGateKeeper();

  // Expose gatekeeper "add" methods as public for unit testing 
  void addContext( const std::string& aId, const ParametersContext_t&);
  void addSettingsContext(const std::string& aId, const SettingsContext_t& aContext);
  void addMasksContext(const std::string& aId, const MasksContext_t& aContext);
  void addDisabledId(const std::string& aId);
};

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_DummyGateKeeper_HPP__ */
