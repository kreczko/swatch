/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MaskingConfigurator.cpp
 * Author: ale
 * 
 * Created on February 15, 2016, 9:02 AM
 */

#include "swatch/system/MaskingConfigurator.hpp"

// SWATCH Headers
#include "swatch/system/System.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"
#include "swatch/dtm/AMCPort.hpp"
#include "swatch/dtm/AMCPortCollection.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/system/Link.hpp"

// Boost Headers
#include <boost/foreach.hpp>

namespace swatch {
namespace system {

MaskingConfigurator::MaskingConfigurator( swatch::system::System& aSystem ) :
mSystem(aSystem)
{
}


void MaskingConfigurator::apply( const FEDEnableMap_t& aFEDEnableMap  )
{
  
  // First pass, disable AMC13s and connected AMCs
  BOOST_FOREACH(const swatch::system::System::CratesMap_t::value_type& c, mSystem.getCrates())
  {
    swatch::system::Crate* lCrate = c.second;

    swatch::dtm::DaqTTCManager* lAmc13 = lCrate->amc13();
    if (!lAmc13) continue;

    // Disable amc13 is FEDEnableMap says so
    FEDEnableMap_t::const_iterator it;
    if ((it = aFEDEnableMap.find(lAmc13->getFedId())) != aFEDEnableMap.end() and it->second == 0) {
//      LOG4CPLUS_INFO(mLogger, "> Masking AMC13 " << lAmc13->getId() << ", FED " << lAmc13->getFedId() << ", value " << it->second);
      lAmc13->disable();
    }

    std::vector<uint32_t> slots = lCrate->getPopulatedSlots();
    // Disable
    if (!lAmc13->getStatus().isEnabled()) {

      BOOST_FOREACH(uint32_t s, slots) {
        if ( s == lAmc13->getSlot() ) continue;
        lCrate->amc(s)->disable();
      }
    }
  }

  // Second pass, disable AMC13 ports
  BOOST_FOREACH(const swatch::system::System::CratesMap_t::value_type& c, mSystem.getCrates())
  {
    swatch::system::Crate* lCrate = c.second;
    swatch::dtm::DaqTTCManager* lAmc13 = lCrate->amc13();

    if (!lAmc13) continue;

    BOOST_FOREACH(swatch::dtm::AMCPort* port, lAmc13->getAMCPorts().getPorts()) {

      swatch::processor::Processor* lAmc = lCrate->amc(port->getSlot());

      // Mask port if slot is not populated or amc is disabled
      if ( !lAmc or !lAmc->getStatus().isEnabled()) {
        port->setMasked();
        //TODO: Dynamic monitoring settings?
        port->setMonitoringStatus(swatch::core::monitoring::kDisabled);
      }

    }
  }

  // Third step, mask input ports linked to disabled procs
  const std::deque<swatch::system::Link*>& lLinks = mSystem.getLinks();
  if ( !lLinks.empty() ) {
    
    BOOST_FOREACH( swatch::system::Link* lLink, mSystem.getLinks() ) {
      if ( lLink->getSrcProcessor()->getStatus().isEnabled() ) continue;
      
      lLink->getDstPort()->setMasked();
      //TODO: Dynamic monitoring settings?
      lLink->getDstPort()->setMonitoringStatus(swatch::core::monitoring::kDisabled);

    }
  }

  // Final step, FEDEnableMap-based masking
  FEDEnableMap_t::const_iterator itFED;
  BOOST_FOREACH( const SystemStub::FEDInputPortsMap::value_type& lPair, mSystem.getStub().connectedFEDs) {
    // Fed is there and is enabled
    if ( (itFED = aFEDEnableMap.find(lPair.first)) != aFEDEnableMap.end() and itFED->second != 0) continue;
    
    BOOST_FOREACH( const std::string& path, lPair.second) {
      swatch::core::MaskableObject* lMaskable = mSystem.getObjPtr<swatch::core::MaskableObject>(path);
      
      // I should really throw here
      if ( !lMaskable ) continue;
      
      lMaskable->setMasked();
      
    }
  }
}


void MaskingConfigurator::clear()
{
  BOOST_FOREACH(const auto& p, mSystem.getActionableChildren())
  {
    p.second->enable();
  }
}  

void MaskingConfigurator::reset(swatch::core::GateKeeper& aGateKeeper) 
{
  // Apply static masks
  BOOST_FOREACH(const auto& p, mSystem.getActionableChildren()) {

    swatch::core::ActionableObject& lAct = *p.second;

    // Disable
    ( aGateKeeper.isEnabled(lAct.getId()) ? lAct.enable() : lAct.disable());

    lAct.resetAndApplyMasks(aGateKeeper);
  }
}


} // namespace system
} //namespace swatch
