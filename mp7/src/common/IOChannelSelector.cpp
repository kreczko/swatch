/* 
 * File:   IOChannelSelector.cpp
 * Author: ale
 * 
 * Created on February 4, 2016, 11:57 AM
 */

#include "swatch/mp7/IOChannelSelector.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/mp7/MP7AbstractProcessor.hpp"

// Boost Headers
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>

// XDAQ Headers
#include <xdata/String.h>

// MP7 Headers
#include "mp7/MP7Controller.hpp"

//log4cplus headers
#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace mp7 {

const channel::Rule_t IOChannelSelector::kAlwaysTrue = boost::lambda::constant(true);
const std::string IOChannelSelector::kIdSelection = "ids";


IOChannelSelector::IOChannelSelector(swatch::core::Command& aCommand) :
  mCommand(aCommand),
  mProcessor(aCommand.getActionable<MP7AbstractProcessor>())
{

}

IOChannelSelector::~IOChannelSelector()
{
}


void IOChannelSelector::addCommandParameters()
{
  mCommand.registerParameter(kIdSelection, xdata::String());
}


//---
std::string
IOChannelSelector::getIdSelection( const swatch::core::XParameterSet& aParams ) const
{
  return aParams.get<xdata::String>(kIdSelection).value_;
}


//---
const channel::Rule_t&
IOChannelSelector::getMaskFilter(const swatch::core::XParameterSet& aParams) const
{
  return kAlwaysTrue;
}


//---
::mp7::ChannelManager
IOChannelSelector::manager(const swatch::core::XParameterSet& aParams) const
{
  // Parse the list of selected ports
  std::set<std::string> lSelIds = swatch::core::toolbox::IdSliceParser::parseSet(getIdSelection(aParams));
  
  channel::DescriptorFinder selector(getDescriptors());
  
  // Get the list of known rx channel ids with a filter
  std::set<std::string> lIds = selector.queryIds(getGroupFilter());
  
  // Ensure that all selected ids are known
  if ( !lSelIds.empty() ) {
    selector.checkExist(lSelIds);
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
::mp7::MP7MiniController&
IOChannelSelector::getDriver() {

  return mProcessor.driver();

}


//
// RxCommandCore
//

const std::string RxChannelSelector::kMaskSelection = "masks";
const std::string RxChannelSelector::kApplyMasks = "apply";
const std::string RxChannelSelector::kInvertMasks = "invert";
const std::string RxChannelSelector::kIgnoreMasks = "ignore";

RxChannelSelector::RxChannelSelector(swatch::core::Command& aCommand, const channel::Rule_t& aFilter) :
  IOChannelSelector(aCommand),
  mRxGroupFilter(aFilter),
  mApplyMaskFilter(!boost::bind(&channel::Descriptor::isMasked, _1)),
  mInvertMaskFilter(boost::bind(&channel::Descriptor::isMasked, _1))
{
}

void RxChannelSelector::addCommandParameters() 
{
  IOChannelSelector::addCommandParameters();
  mCommand.registerParameter(kMaskSelection, xdata::String(kApplyMasks));
}


const channel::Rule_t& RxChannelSelector::getGroupFilter() const
{
  return mRxGroupFilter;
}

const channel::DescriptorMap_t& RxChannelSelector::getDescriptors() const
{
  return mProcessor.getRxDescriptors();
}

const channel::Rule_t& RxChannelSelector::getMaskFilter(const swatch::core::XParameterSet& aParams) const
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

TxChannelSelector::TxChannelSelector(swatch::core::Command& aCommand, const channel::Rule_t& aFilter) :
  IOChannelSelector(aCommand),
  mTxGroupFilter(aFilter)
{
}

const channel::Rule_t& TxChannelSelector::getGroupFilter() const
{
  return mTxGroupFilter;
}


const channel::DescriptorMap_t& TxChannelSelector::getDescriptors() const
{
  return mProcessor.getTxDescriptors();
}

//
// RxMGTCommandCore
//
RxMGTSelector::RxMGTSelector(swatch::core::Command& aCommand) :
  RxChannelSelector(aCommand, boost::bind(&channel::Descriptor::hasMGT, _1))
{
}

//
// TxMGTCommandCore
//

TxMGTSelector::TxMGTSelector(swatch::core::Command& aCommand) :
  TxChannelSelector(aCommand, boost::bind(&channel::Descriptor::hasMGT, _1))
{
}


//
// RxBufferCommandCore
//

RxBufferSelector::RxBufferSelector(swatch::core::Command& aCommand) :
  RxChannelSelector(aCommand, boost::bind(&channel::Descriptor::hasBuffer, _1))
{
}


//
// TxBufferCommandCore
//

TxBufferSelector::TxBufferSelector(swatch::core::Command& aCommand) :
  TxChannelSelector(aCommand, boost::bind(&channel::Descriptor::hasBuffer, _1))
{
}



} // namespace mp7
} // namespace swatch
