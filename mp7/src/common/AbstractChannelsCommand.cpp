
#include "swatch/mp7/AbstractChannelsCommand.hpp"


// XDAQ headers
#include "xdata/String.h"

// mp7 headers
#include "mp7/MP7Controller.hpp"
#include "mp7/Orbit.hpp"

// SWATCH headers
#include "swatch/mp7/MP7Processor.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/toolbox/IntListParser.hpp"


namespace swatch {
namespace mp7 {

namespace orbit {
// --------------------------------------------------------
bool
isValid( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric ) {
  return not (
        (aBx.isNaN() or aBx.value_ >= aMetric.bunchCount()) or
        (aCycle.isNaN() or aCycle.value_ >= aMetric.clockRatio())
      );

}


// --------------------------------------------------------
bool
isGood( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric ) {
  return not (aBx.isNaN() or aBx.value_ >= aMetric.bunchCount()) or
         (aCycle.isNaN() or aCycle.value_ >= aMetric.clockRatio());
}


// --------------------------------------------------------
bool
isNull( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle ) {
  return ( aBx.isNaN() and aCycle.isNaN() );
}

} // namespace orbit


AbstractChannelsCommand::~AbstractChannelsCommand() {
}

// --------------------------------------------------------
std::vector<uint32_t>
AbstractChannelsCommand::getPortIds() const {
  // Get the resource
  const swatch::mp7::MP7Processor* p = getParent<swatch::mp7::MP7Processor>();

  // Pick the right set of ports
  const std::vector<swatch::processor::ProcessorPortStub>* stubs;
  switch ( mGroup ) {
    case kRx:
      stubs = &(p->getStub().rxPorts);
      break;
    case kTx:
      stubs = &(p->getStub().txPorts);
      break;
    default:
      throw std::runtime_error("Aaaaargh");
  }

  // Pull out the hardware ids
  std::vector<uint32_t> ids;
  for(auto lIt=stubs->begin();  lIt!=stubs->end(); lIt++)
    ids.push_back(lIt->number);

  return ids;
}

// --------------------------------------------------------
::mp7::ChannelsManager 
AbstractChannelsCommand::getChannelsMgr(const swatch::core::XParameterSet& aParams) {

  // Parse parameter into channel ID mask
  std::string channelMask = aParams.get<xdata::String>(kPortSelection).value_;

  // Grab list of ports registered in the processor
  ::mp7::MP7Controller& driver = getParent<swatch::mp7::MP7Processor>()->driver();

  // If the mask is empty, return the full set of known ports
  if ( channelMask.empty() )
    return driver.channelMgr( getPortIds() );

  // Finally parse the list of channels
  std::vector<uint32_t> enableMask = swatch::core::toolbox::UIntListParser::parse(channelMask);

  std::vector<uint32_t> stubIds = getPortIds();
  // Sort the lists 
  std::sort(enableMask.begin(), enableMask.end());
  std::sort(stubIds.begin(), stubIds.end());

  // Obtain intersection of the two lists
  std::vector<uint32_t> intersection;
  std::set_intersection(enableMask.begin(), enableMask.end(), stubIds.begin(), stubIds.end(), std::back_inserter(intersection));

  return driver.channelMgr(intersection);
}

const std::string AbstractChannelsCommand::kPortSelection = "portSelection";


} //end ns mp7
} //end ns swatch
