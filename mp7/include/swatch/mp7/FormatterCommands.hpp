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
#include "swatch/mp7/CommandChannelSelector.hpp"


namespace swatch {
namespace mp7 {

class TDRFormatterCommand : public swatch::core::Command {
public:
  /**
   * 
   * @param aId
   * @param aActionable
   */
  TDRFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~TDRFormatterCommand() {}
  
  /**
   * 
   * @param params
   * @return 
   */
  virtual State code(const ::swatch::core::XParameterSet& params);
  
private:
  //! Command core
  RxChannelSelector mFmtSelector;
};

/**
 * @class DemuxFormatterCommand
 */
class DemuxFormatterCommand : public swatch::core::Command {
public:

  DemuxFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);

  virtual ~DemuxFormatterCommand() {}

  virtual State code(const ::swatch::core::XParameterSet& params);

private:
  //! Command core
  RxChannelSelector mFmtSelector;
};


/**
 * @class S1Formatter
 */
class S1Formatter : public swatch::core::Command {
public:

  S1Formatter(const std::string& aId, swatch::core::ActionableObject& aActionable);

  virtual ~S1Formatter() {}

  virtual State code(const ::swatch::core::XParameterSet& params);

private:
  RxChannelSelector mFmtSelector;  
};

}
}

#endif	/* __SWATCH_MP7_FORMATTERCOMMANDS_HPP__ */

