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

class DummyGateKeeper : public core::GateKeeper {
  public:

  /// Constructor
  DummyGateKeeper();

  /// Destructor
  virtual ~DummyGateKeeper();

  // Expose gatekeeper "add" method as public for unit testing 
  void addTable( const std::string& aId, tTable&);
};

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_DummyGateKeeper_HPP__ */
