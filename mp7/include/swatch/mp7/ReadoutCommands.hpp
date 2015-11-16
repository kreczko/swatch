/* 
 * File:   Readout.hpp
 * Author: ale
 *
 * Created on October 2, 2015, 4:56 PM
 */

#ifndef __SWATCH_MP7_READOUTCOMMANDS_HPP__
#define	__SWATCH_MP7_READOUTCOMMANDS_HPP__


#include <stdint.h>

#include "swatch/core/Command.hpp"

#include "mp7/ReadoutCtrlNode.hpp"

namespace mp7 {
class MP7Controller;
}

namespace swatch {
namespace mp7 {


class SetupReadout : public core::Command {
public:
  SetupReadout( const std::string& aId, swatch::core::ActionableObject& aActionable );
  virtual ~SetupReadout();
  virtual core::Command::State code(const ::swatch::core::XParameterSet& aParams);
};


class LoadReadoutMenu : public core::Command {
public:
  LoadReadoutMenu( const std::string& aId, swatch::core::ActionableObject& aActionable );
  virtual ~LoadReadoutMenu();
  virtual core::Command::State code(const ::swatch::core::XParameterSet& aParams);

//  static LoadReadoutMenu* create( const std::string& aId, ::mp7::MP7Controller& aDriver );

private:
  uint32_t mBanks;
  uint32_t mModes;
  uint32_t mCaptures;
};


} //end ns mp7
} //end ns swatch


#endif	/* __SWATCH_MP7_READOUTCOMMANDS_HPP__ */


