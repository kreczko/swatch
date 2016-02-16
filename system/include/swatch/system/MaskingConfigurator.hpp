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
#include <string>
#include <deque>

#include <boost/noncopyable.hpp>

namespace swatch {
// Forward declaration
namespace core {
class GateKeeper;
}

namespace system {
class System;

class MaskingConfigurator : boost::noncopyable {
public:
  
  typedef std::map<uint32_t, uint32_t> FEDEnableMap_t;

  MaskingConfigurator( swatch::system::System& aSystem );
  
  /**
   * Applies static masks from GateKeeper
   * @param aGateKeeper GateKeeper reference
   */
  void applyStatic(swatch::core::GateKeeper& aGateKeeper);
  
  /**
   * Applies FED-mask-based dynamic masks
   * @param aFEDEnableMap FEDEnableMap, listing enable and disabled FEDs
   */
  void applyDynamic( const FEDEnableMap_t& aFEDEnableMap );
  
  /**
   * Clears masks
   */
  void clear();
  
  const std::deque<std::string>& getAutoDisabledDTMs() const;
  const std::deque<std::string>& getAutoDisabledAMCs() const;
  const std::deque<std::string>& getAutoDisabledAMCPorts() const;
  const std::deque<std::string>& getAutoMaskedLinks() const;
  const std::deque<std::string>& getAutoMaskedInputs() const;
  
private:
  
  void clearLists();

  void disableCrates(const FEDEnableMap_t& aFEDEnableMap);

  void maskAMCPorts();

  void maskInternalLinks();
  
  void maskExternalInputs( const FEDEnableMap_t& aFEDEnableMap );
  
  swatch::system::System& mSystem;
  
  //! List of automatically disabled DTMs
  std::deque<std::string> mAutoDisabledDtms;
  
  //! List of automatically disabled AMCs
  std::deque<std::string> mAutoDisabledAMCs;
  
  //! List of automatically disabled AMC ports
  std::deque<std::string> mAutoDisabledAMCPorts;
  
  //! List of automatically masked internal links
  std::deque<std::string> mAutoMaskedLinks;
  
  //! List of automatically disable inputs
  std::deque<std::string> mAutoMaskedInputs;

};

} // namespace system
} //namespace swatch

#endif /* __SWATCH_SYSTEM_MASKINGCONFIGURATOR_HPP__ */

