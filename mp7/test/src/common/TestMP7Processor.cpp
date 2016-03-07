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
  swatch::core::Command(aId, aResource, xdata::String()),
  mCore(*this)
{
  mCore.addCommandParameters();
}


swatch::core::Command::State PrintRxDescriptors::code(const core::XParameterSet& aParams)
{
  ::mp7::ChannelsManager mgr = mCore.manager(aParams);
  
  auto channels = mgr.ids(::mp7::kBufferIDs).channels();
  std::ostringstream res;
  res << "Selected channels : ";
  std::copy(channels.begin(), channels.end(), std::ostream_iterator<uint32_t>(res, " "));
  setResult(xdata::String(res.str()));
  return State::kWarning;
}

const std::string TmtParameterMapper::kBxParam = "bx";
const std::string TmtParameterMapper::kCycleParam = "cycle";
const std::string TmtParameterMapper::kTmtBxParam = "tmtBx";
const std::string TmtParameterMapper::kTmtCycleParam = "tmtCycle";
const std::string TmtParameterMapper::kTmtIdParam = "tmtId";

void TmtParameterMapper::replaceParameters(swatch::core::Command& aCommand)
{
  aCommand.unregisterParameter(kBxParam);
  aCommand.unregisterParameter(kCycleParam);

  aCommand.registerParameter(kTmtBxParam, xdata::UnsignedInteger(0x0));
  aCommand.registerParameter(kTmtCycleParam, xdata::UnsignedInteger(0x0));
  aCommand.registerParameter(kTmtIdParam, xdata::UnsignedInteger(0x0));
}


core::ReadOnlyXParameterSet TmtParameterMapper::rebuild(const core::ReadOnlyXParameterSet& aParams, const ::mp7::orbit::Metric& aMetric)
{
  xdata::UnsignedInteger tmtBx = aParams.get<xdata::UnsignedInteger>(kTmtBxParam);
  xdata::UnsignedInteger tmtCycle = aParams.get<xdata::UnsignedInteger>(kTmtCycleParam);
  xdata::UnsignedInteger tmtSlot = aParams.get<xdata::UnsignedInteger>(kTmtIdParam);
  
  // Copy the parameter set
  swatch::core::ReadOnlyXParameterSet lParams(aParams);
  
  // Strip parameters
  lParams.erase(kTmtBxParam);
  lParams.erase(kTmtCycleParam);
  lParams.erase(kTmtIdParam);
  
  
  
  if ( !orbit::isValid( tmtBx, tmtCycle, aMetric ) ) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << tmtBx << ", " << tmtCycle << ")";

    throw orbit::PointError(msg.str());
  }

  
  ::mp7::orbit::Point p(tmtBx, tmtCycle);
  aMetric.addBXs(p, tmtSlot);
  
  boost::shared_ptr<xdata::UnsignedInteger> lBx( new xdata::UnsignedInteger(p.bx) );
  boost::shared_ptr<xdata::UnsignedInteger> lCycle( new xdata::UnsignedInteger(p.cycle) );
  
  lParams.adopt(kBxParam, lBx);
  lParams.adopt(kCycleParam, lCycle);

  return lParams;
}

//
// Parameter override tests
//
//---
TMTAlignCommand::TMTAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
AlignRxsToCommand(aId, aActionable)
{
  TmtParameterMapper::replaceParameters(*this);
}


//---
core::Functionoid::State TMTAlignCommand::code(const ::swatch::core::XParameterSet& aParams)
{
  // FIXME: Embed this check somewhere
  ::mp7::MP7Controller& driver = getActionable<MP7AbstractProcessor>().driver();
  ::mp7::orbit::Metric lMetric = driver.getMetric();
  
  swatch::core::ReadOnlyXParameterSet lParams = TmtParameterMapper::rebuild(aParams, lMetric);

  /*
    
  xdata::UnsignedInteger tmtBx = aParams.get<xdata::UnsignedInteger>(kTmtBxParam);
  xdata::UnsignedInteger tmtCycle = aParams.get<xdata::UnsignedInteger>(kTmtCycleParam);
  xdata::UnsignedInteger tmtSlot = aParams.get<xdata::UnsignedInteger>(kTmtIdParam);
  
  // Copy the parameter set
  swatch::core::ReadOnlyXParameterSet lParams(aParams);
  
  // Strip parameters
  lParams.erase(kTmtBxParam);
  lParams.erase(kTmtCycleParam);
  lParams.erase(kTmtIdParam);
  
  
  
  if ( !orbit::isValid( tmtBx, tmtCycle, lMetric ) ) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << tmtBx << ", " << tmtCycle << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }

  
  
  ::mp7::orbit::Point p(tmtBx, tmtCycle);
  lMetric.addBXs(p, tmtSlot);
  
  boost::shared_ptr<xdata::UnsignedInteger> lBx( new xdata::UnsignedInteger(p.bx) );
  boost::shared_ptr<xdata::UnsignedInteger> lCycle( new xdata::UnsignedInteger(p.cycle) );
  
  lParams.adopt(kBxParam, lBx);
  lParams.adopt(kCycleParam, lCycle);
  */
  return AlignRxsToCommand::code(lParams);  
}

} // namespace test
} // namespace mp7
} // namespace swatch