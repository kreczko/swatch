/* 
 * File:   FormatterCommands.hpp
 * Author: tom
 *
 * Created on 13 October 2015, 09:51
 */

#ifndef __SWATCH_MP7_FORMATTERCOMMANDS_HPP__
#define	__SWATCH_MP7_FORMATTERCOMMANDS_HPP__


#include <string>

#include "xdata/String.h"

#include "swatch/core/Command.hpp"


namespace mp7 {
class ChannelsManager;
}


namespace swatch {
namespace mp7 {


class AbstractFormatterCommand : public core::Command {
public:
  virtual ~AbstractFormatterCommand();

protected:
  template<typename ResultType>
  AbstractFormatterCommand( const std::string& aId, const ResultType& aDefault );

  ::mp7::ChannelsManager getChannelsMgr(const swatch::core::XParameterSet& aParams);

 private:
  static const std::string kPortSelection;
};


template<typename ResultType>
AbstractFormatterCommand::AbstractFormatterCommand( const std::string& aId , const ResultType& aDefault ) :
  core::Command(aId, aDefault)
{
  registerParameter(kPortSelection,xdata::String());
}


class TDRFormatterCommand : public AbstractFormatterCommand {
public:
  TDRFormatterCommand(const std::string& aId);
  virtual ~TDRFormatterCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);
};


}
}

#endif	/* __SWATCH_MP7_FORMATTERCOMMANDS_HPP__ */

