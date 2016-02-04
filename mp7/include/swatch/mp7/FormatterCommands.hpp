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
#include "swatch/mp7/ChannelCommandBase.hpp"


namespace swatch {
namespace mp7 {

class TDRFormatterCommand : public ChannelCommandBase {
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
  RxCommandCore mFmtCore;
};


}
}

#endif	/* __SWATCH_MP7_FORMATTERCOMMANDS_HPP__ */

