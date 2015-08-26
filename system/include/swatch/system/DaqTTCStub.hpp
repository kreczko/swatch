/**
 * @file    DAQTTCStub.hpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_DAQTTCSTUB_HPP__
#define __SWATCH_SYSTEM_DAQTTCSTUB_HPP__


// C++ headers
#include <iosfwd>
#include <stdint.h>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"


namespace swatch {
namespace system {


//! Structure that holds the data required to build a DaqTTCManager
class DaqTTCStub : public swatch::core::AbstractStub {
public:

  DaqTTCStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~DaqTTCStub() { }

  //! Class to create the DaqTTC object
  std::string creator;

  //! Role of the DaqTTC object
  std::string role;
  
  //! Uri to access the hardware resource
  std::string uriT1;

  //! Address table
  std::string addressTableT1;

  //! Uri to access the hardware resource
  std::string uriT2;

  //! Address table
  std::string addressTableT2;

  //! Id of the uTCA crate where the DaqTTC is installed 
  std::string crate;

  //! Slot where the board is installed 
  uint32_t slot;

  //! ID for this FED
  uint16_t fedId;

};


std::ostream& operator<<(std::ostream& os, const swatch::system::DaqTTCStub& sd);


} // namespace system
} // namespace swatch


#endif /* __SWATCH_SYSTEM_DAQTTCSTUB_HPP__ */
