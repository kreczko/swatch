/**
 * @file    DAQTTCStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_DAQTTCSTUB_HPP__
#define __SWATCH_SYSTEM_DAQTTCSTUB_HPP__

// C++ Headers
#include <ostream>
#include <stdint.h>
#include <vector>

// Swatch Headers
#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace system {

/**
 * @class DaqTTCStub
 * @brief Struct to hold the data to construct a DaqTTCManager object
 */
class DaqTTCStub : public swatch::core::AbstractStub {
public:

  DaqTTCStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~DaqTTCStub() { }

  //! Class to create the DaqTTC object
  std::string creator;

  //! Id of the uTCA crate where the DaqTTC is installed 
  std::string crate;

  //! Slot where the board is installed 
  uint32_t slot;

  //! Uri to access the hardware resource
  std::string uriT1;

  //! Address table
  std::string addressTableT1;

  //! Uri to access the hardware resource
  std::string uriT2;

  //! Address table
  std::string addressTableT2;

};

//typedef xdata::Bag<DaqTTCStub> DaqTTCBag;


std::ostream& operator<<(std::ostream& os, const swatch::system::DaqTTCStub& sd);


} // namespace system
} // namespace swatch


#endif /* __SWATCH_SYSTEM_AMC13SERVICESTUB__ */