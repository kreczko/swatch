/* 
 * File:   TestMP7Processor.cpp
 * Author: ale
 * 
 * Created on December 8, 2015, 2:19 PM
 */


#include "swatch/mp7/test/TestMP7Processor.hpp"

#include "swatch/core/Factory.hpp"

#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>

#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/mp7/MP7Ports.hpp"
#include "swatch/mp7/Utilities.hpp"

#include <boost/foreach.hpp>
#include <xdata/Vector.h>
#include "mp7/definitions.hpp"
#include "mp7/ChannelsManager.hpp"
#include "mp7/MP7Controller.hpp"
#include "swatch/mp7/MGTCommands.hpp"
#include "swatch/core/ReadWriteXParameterSet.hpp"

#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/copy.hpp>

SWATCH_REGISTER_CLASS(swatch::mp7::test::TestMP7Processor);

namespace swatch {
namespace mp7 {
namespace test {


//---
TestMP7Processor::TestMP7Processor(const swatch::core::AbstractStub& aStub) :
MP7NullAlgoProcessor(aStub)
{
  // registerCommand<PrintRxDescriptors>("printRxDescriptors");
  registerCommand<TMTAlignCommand>("tmtAlign");

}

//---
TestMP7Processor::~TestMP7Processor()
{
}

PrintRxDescriptors::PrintRxDescriptors(const std::string& aId, core::ActionableObject& aResource) :
  ChannelCommandBase(aId, aResource, xdata::String()),
  mCore(*this)
{
  mCore.addParameters();
}


swatch::core::Command::State PrintRxDescriptors::code(const core::XParameterSet& aParams)
{
  ::mp7::ChannelsManager mgr = mCore.getManager(aParams);
  
  auto channels = mgr.ids(::mp7::kBufferIDs).channels();
  std::ostringstream res;
  res << "Selected channels : ";
  std::copy(channels.begin(), channels.end(), std::ostream_iterator<uint32_t>(res, " "));
  setResult(xdata::String(res.str()));
  return State::kWarning;
}

/*
swatch::core::Command::State PrintRxDescriptors::code(const core::XParameterSet& aParams)
{
  // fetch the port selection parameter
  const std::string portSelection = aParams.get<xdata::String>(kPortSelection).value_;
  
  // Parse the list of selected ports
  std::set<std::string> lSelIds = swatch::core::toolbox::IdSliceParser::parseSet(portSelection);
  
  // Fetch the processor
  MP7Processor& lProcessor = getActionable<MP7Processor>();
  const ChannelsMap_t& rxDesc = lProcessor.getRxDescriptors();
  
  DescriptorSelector selector(rxDesc);
  
  // Get the list of known rx channel ids with a filter
  std::set<std::string> lIds = selector.getIds();
  
  // Ensure that all selected ids are known
  if ( !lSelIds.empty() ) {
    selector.checkAvailable(lSelIds);
    
  } else {
    lSelIds = lIds;
  }

  // Apply mask on ports
  auto notMasked = (! boost::bind(&ChannelDescriptor::isMasked, _1) );
  std::set<std::string> lEnabledIds = selector.filterIds(lSelIds, notMasked);
  
  // Convert obj ids to channel ids
  auto channels = selector.mapIdsToChannels(lEnabledIds);

  std::ostringstream res;
  res << "Selected channels : ";
  std::copy(channels.begin(), channels.end(), std::ostream_iterator<uint32_t>(res, " "));
  setResult(xdata::String(res.str()));

  return State::kDone;
}
*/


//
// Parameter override tests
//
//---
TMTAlignCommand::TMTAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
AlignRxsToCommand(aId, aActionable)
{
  unregisterParameter("bx");
  unregisterParameter("cycle");

  registerParameter("tmtBx", xdata::UnsignedInteger(0x0));
  registerParameter("tmtCycle", xdata::UnsignedInteger(0x0));
  registerParameter("tmtSlot", xdata::UnsignedInteger(0x0));
}


//---
core::Functionoid::State TMTAlignCommand::code(const ::swatch::core::XParameterSet& aParams)
{
  // FIXME: Embed this check somewhere
  ::mp7::MP7Controller& driver = getActionable<MP7AbstractProcessor>().driver();
  ::mp7::orbit::Metric metric = driver.getMetric();
  
  xdata::UnsignedInteger tmtBx = aParams.get<xdata::UnsignedInteger>("tmtBx");
  xdata::UnsignedInteger tmtCycle = aParams.get<xdata::UnsignedInteger>("tmtCycle");
  xdata::UnsignedInteger tmtSlot = aParams.get<xdata::UnsignedInteger>("tmtSlot");
  
  // Copy the parameter set
  swatch::core::ReadOnlyXParameterSet lParams(aParams);
  
  // Strip parameters
  lParams.erase("tmtBx");
  lParams.erase("tmtCycle");
  lParams.erase("tmtSlot");
  
  
  
  if ( !orbit::isValid( tmtBx, tmtCycle, metric ) ) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << tmtBx << ", " << tmtCycle << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }

  
  
  ::mp7::orbit::Point p(tmtBx, tmtCycle);
  metric.addBXs(p, tmtSlot);
  
  boost::shared_ptr<xdata::UnsignedInteger> lBx( new xdata::UnsignedInteger(p.bx) );
  boost::shared_ptr<xdata::UnsignedInteger> lCycle( new xdata::UnsignedInteger(p.cycle) );
  
  lParams.adopt("bx", lBx);
  lParams.adopt("cycle", lCycle);
  
  return AlignRxsToCommand::code(lParams);  
}

} // namespace test
} // namespace mp7
} // namespace swatch