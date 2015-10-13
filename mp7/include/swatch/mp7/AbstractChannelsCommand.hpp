#ifndef __SWATCH_MP7_MP7CHANNELSCOMMAND_HPP__
#define __SWATCH_MP7_MP7CHANNELSCOMMAND_HPP__

#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"

#include "mp7/definitions.hpp"
#include "mp7/PathConfigurator.hpp"

#include "swatch/core/Command.hpp"


namespace mp7 {
class ChannelsManager;
}


namespace swatch {
namespace mp7 {

    
// Orbit related functions
// TODO: move them into a separate library?
namespace orbit {

bool isValid( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric );
bool isGood( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric );
bool isNull( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle);

}


class AbstractChannelsCommand : public ::swatch::core::Command {
public:
  virtual ~AbstractChannelsCommand();
  
  // Dedicated channel group accessible to derived classes only 
  enum ChannelGroup {
    kRx,
    kTx
  };

protected:

  template<typename ResultType>
  AbstractChannelsCommand( const std::string& aId, ChannelGroup aGroup, const ResultType& aDefault );

  ::mp7::ChannelsManager getChannelsMgr(const swatch::core::XParameterSet& aParams);

  const ChannelGroup mGroup;
  
private:

  std::vector<uint32_t> getPortIds() const;

  static const std::string kPortSelection;  
};


template<typename ResultType>
AbstractChannelsCommand::AbstractChannelsCommand( const std::string& aId , ChannelGroup aGroup, const ResultType& aDefault ) :
  swatch::core::Command(aId, aDefault),
  mGroup(aGroup) {
  registerParameter(kPortSelection,xdata::String());
}


}
}

#endif /* __SWATCH_MP7_MP7CHANNELSCOMMAND_HPP__ */
