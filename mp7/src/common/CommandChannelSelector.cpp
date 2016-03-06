/* 
 * File:   ChannelCommandBase.cpp
 * Author: ale
 * 
 * Created on February 4, 2016, 11:57 AM
 */

#include "swatch/mp7/CommandChannelSelector.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/mp7/MP7AbstractProcessor.hpp"

#include <boost/lambda/lambda.hpp>
#include <xdata/String.h>

#include "mp7/MP7Controller.hpp"

//log4cplus headers
#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace mp7 {

const Rule_t CommandChannelSelector::kAlwaysTrue = boost::lambda::constant(true);
const std::string CommandChannelSelector::kIdSelection = "ids";


CommandChannelSelector::CommandChannelSelector(swatch::core::Command& aCommand) :
  mCommand(aCommand),
  mProcessor(aCommand.getActionable<MP7AbstractProcessor>())
{

}

CommandChannelSelector::~CommandChannelSelector()
{
}


void CommandChannelSelector::addParameters()
{
  mCommand.registerParameter(kIdSelection, xdata::String());
}


//---
std::string
CommandChannelSelector::getIdSelection( const swatch::core::XParameterSet& aParams ) const
{
  return aParams.get<xdata::String>(kIdSelection).value_;
}


//---
const Rule_t&
CommandChannelSelector::getMaskFilter(const swatch::core::XParameterSet& aParams) const
{
  return kAlwaysTrue;
}


//---
::mp7::ChannelsManager
CommandChannelSelector::getManager(const swatch::core::XParameterSet& aParams) const
{
  // Parse the list of selected ports
  std::set<std::string> lSelIds = swatch::core::toolbox::IdSliceParser::parseSet(getIdSelection(aParams));
  
  DescriptorSelector selector(getChannelDescriptors());
  
  // Get the list of known rx channel ids with a filter
  std::set<std::string> lIds = selector.queryIds(getGroupFilter());
  
  // Ensure that all selected ids are known
  if ( !lSelIds.empty() ) {
    selector.checkAvailable(lSelIds);
  } else {
    lSelIds = lIds;
  }

  // Apply mask on ports
  std::set<std::string> lEnabledIds = selector.filterIds(lSelIds, getMaskFilter(aParams));
  
  // Convert obj ids to channel ids
  std::vector<uint32_t> lChannels = selector.mapIdsToChannels(lEnabledIds);
  
  std::ostringstream msg;
  boost::copy( lEnabledIds, std::ostream_iterator<std::string>(msg," "));

  LOG4CPLUS_DEBUG(mProcessor.getLogger(),"Command '"<< mCommand.getId() << "', selected IDs " << msg.str());

  return mProcessor.driver().channelMgr(lChannels);

}

//---
::mp7::MP7Controller&
CommandChannelSelector::getDriver() {

  return mProcessor.driver();

}


//
// RxCommandCore
//

const std::string RxChannelSelector::kMaskSelection = "masks";
const std::string RxChannelSelector::kApplyMasks = "apply";
const std::string RxChannelSelector::kInvertMasks = "invert";
const std::string RxChannelSelector::kIgnoreMasks = "ignore";

RxChannelSelector::RxChannelSelector(swatch::core::Command& aCommand, const Rule_t& aFilter) :
  CommandChannelSelector(aCommand),
  mRxGroupFilter(aFilter),
  mApplyMaskFilter(!boost::bind(&ChannelDescriptor::isMasked, _1)),
  mInvertMaskFilter(boost::bind(&ChannelDescriptor::isMasked, _1))
{
}

void RxChannelSelector::addParameters() 
{
  CommandChannelSelector::addParameters();
  mCommand.registerParameter(kMaskSelection, xdata::String(kApplyMasks));
}


const Rule_t& RxChannelSelector::getGroupFilter() const
{
  return mRxGroupFilter;
}

const ChannelsMap_t& RxChannelSelector::getChannelDescriptors() const
{
  return mProcessor.getRxDescriptors();
}

const Rule_t& RxChannelSelector::getMaskFilter(const swatch::core::XParameterSet& aParams) const
{
  const std::string& masks = aParams.get<xdata::String>(kMaskSelection).value_;
  
  if ( masks == kApplyMasks ) {
    return mApplyMaskFilter;
  } else if ( masks == kInvertMasks ) {
    return mInvertMaskFilter;
  } else if ( masks == kIgnoreMasks ) {
    return kAlwaysTrue;
  } else {
    throw ApplyMaskOptionInvalid("Unknown option "+masks);
  }
}

//
// TxCommandCore
//

TxChannelSelector::TxChannelSelector(swatch::core::Command& aCommand, const Rule_t& aFilter) :
  CommandChannelSelector(aCommand),
  mTxGroupFilter(aFilter)
{
}

const Rule_t& TxChannelSelector::getGroupFilter() const
{
  return mTxGroupFilter;
}


const ChannelsMap_t& TxChannelSelector::getChannelDescriptors() const
{
  return mProcessor.getTxDescriptors();
}

//
// RxMGTCommandCore
//
RxMGTSelector::RxMGTSelector(swatch::core::Command& aCommand) :
  RxChannelSelector(aCommand, boost::bind(&ChannelDescriptor::hasMGT, _1))
{
}

//
// TxMGTCommandCore
//

TxMGTSelector::TxMGTSelector(swatch::core::Command& aCommand) :
  TxChannelSelector(aCommand, boost::bind(&ChannelDescriptor::hasMGT, _1))
{
}


//
// RxBufferCommandCore
//

RxBufferSelector::RxBufferSelector(swatch::core::Command& aCommand) :
  RxChannelSelector(aCommand, boost::bind(&ChannelDescriptor::hasBuffer, _1))
{
}


//
// TxBufferCommandCore
//

TxBufferSelector::TxBufferSelector(swatch::core::Command& aCommand) :
  TxChannelSelector(aCommand, boost::bind(&ChannelDescriptor::hasBuffer, _1))
{
}



} // namespace mp7
} // namespace swatch
