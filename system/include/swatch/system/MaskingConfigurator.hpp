/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MaskingConfigurator.hpp
 * Author: ale
 *
 * Created on February 15, 2016, 9:02 AM
 */

#ifndef __SWATCH_SYSTEM_MASKINGCONFIGURATOR_HPP__
#define __SWATCH_SYSTEM_MASKINGCONFIGURATOR_HPP__

// C++ Headers
#include <map>
#include <stdint.h>

namespace swatch {
// Forward declaration
namespace core {
class GateKeeper;
}

namespace system {
class System;

class MaskingConfigurator {
public:
  
  typedef std::map<uint32_t, uint32_t> FEDEnableMap_t;

  MaskingConfigurator( swatch::system::System& aSystem );
  
  void reset(swatch::core::GateKeeper& aGateKeeper);
  void apply( const FEDEnableMap_t& aFEDEnableMap );
  void clear();
  
private:
  swatch::system::System& mSystem;
};

} // namespace system
} //namespace swatch

#endif /* __SWATCH_SYSTEM_MASKINGCONFIGURATOR_HPP__ */

