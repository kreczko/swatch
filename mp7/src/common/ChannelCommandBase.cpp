/* 
 * File:   ChannelCommandBase.cpp
 * Author: ale
 * 
 * Created on February 4, 2016, 11:57 AM
 */

#include "swatch/mp7/ChannelCommandBase.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/mp7/MP7AbstractProcessor.hpp"

#include <boost/lambda/lambda.hpp>
#include <xdata/String.h>

#include "mp7/MP7Controller.hpp"

namespace swatch {
namespace mp7 {

ChannelCommandBase::~ChannelCommandBase()
{
}

const Rule_t ChannelCommandCore::kAlwaysTrue = boost::lambda::constant(true);
const std::string ChannelCommandCore::kIdSelection = "ids";


ChannelCommandCore::ChannelCommandCore(ChannelCommandBase& aCommand) :
  mCommand(aCommand),
  mProcessor(aCommand.getActionable<MP7AbstractProcessor>())
{

}

ChannelCommandCore::~ChannelCommandCore()
{
}


void ChannelCommandCore::addParameters()
{
  registerParameter(kIdSelection, xdata::String());
}


std::string ChannelCommandCore::getIdSelection( const swatch::core::XParameterSet& aParams ) const
{
  return aParams.get<xdata::String>(kIdSelection).value_;
}


const Rule_t& ChannelCommandCore::getMaskFilter(const swatch::core::XParameterSet& aParams) const
{
  return kAlwaysTrue;
}


::mp7::ChannelsManager ChannelCommandCore::getManager(const swatch::core::XParameterSet& aParams) const
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
  
  return mCommand.getActionable<MP7AbstractProcessor>().driver().channelMgr(lChannels);

}

//
// RxCommandCore
//

const std::string RxCommandCore::kMaskSelection = "masks";
const std::string RxCommandCore::kApplyMasks = "apply";
const std::string RxCommandCore::kInvertMasks = "invert";
const std::string RxCommandCore::kIgnoreMasks = "ignore";

RxCommandCore::RxCommandCore(ChannelCommandBase& aCommand, const Rule_t& aFilter) :
  ChannelCommandCore(aCommand),
  mRxGroupFilter(aFilter),
  mApplyMaskFilter(!boost::bind(&ChannelDescriptor::isMasked, _1)),
  mInvertMaskFilter(boost::bind(&ChannelDescriptor::isMasked, _1))
{
}

void RxCommandCore::addParameters() 
{
  ChannelCommandCore::addParameters();
  registerParameter(kMaskSelection, xdata::String(kApplyMasks));
}


const Rule_t& RxCommandCore::getGroupFilter() const
{
  return mRxGroupFilter;
}

const ChannelsMap_t& RxCommandCore::getChannelDescriptors() const
{
  return mProcessor.getRxDescriptors();
}

const Rule_t& RxCommandCore::getMaskFilter(const swatch::core::XParameterSet& aParams) const
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

TxCommandCore::TxCommandCore(ChannelCommandBase& aCommand, const Rule_t& aFilter) :
  ChannelCommandCore(aCommand),
  mTxGroupFilter(aFilter)
{
}

const Rule_t& TxCommandCore::getGroupFilter() const
{
  return mTxGroupFilter;
}


const ChannelsMap_t& TxCommandCore::getChannelDescriptors() const
{
  return mProcessor.getTxDescriptors();
}

//
// RxMGTCommandCore
//
RxMGTCommandCore::RxMGTCommandCore(ChannelCommandBase& aCommand) :
  RxCommandCore(aCommand, boost::bind(&ChannelDescriptor::hasMGT, _1))
{
}

//
// TxMGTCommandCore
//

TxMGTCommandCore::TxMGTCommandCore(ChannelCommandBase& aCommand) :
  TxCommandCore(aCommand, boost::bind(&ChannelDescriptor::hasMGT, _1))
{
}


//
// RxBufferCommandCore
//

RxBufferCommandCore::RxBufferCommandCore(ChannelCommandBase& aCommand) :
  RxCommandCore(aCommand, boost::bind(&ChannelDescriptor::hasBuffer, _1))
{
}


//
// TxBufferCommandCore
//

TxBufferCommandCore::TxBufferCommandCore(ChannelCommandBase& aCommand) :
  TxCommandCore(aCommand, boost::bind(&ChannelDescriptor::hasBuffer, _1))
{
}



} // namespace mp7
} // namespace swatch
