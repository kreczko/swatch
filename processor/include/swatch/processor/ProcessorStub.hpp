/**
 * @file    ProcessorStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    09/11/14
 */


#ifndef __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__
#define __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__

// C++ Headers
#include <ostream>
#include <vector>
// Swatch Headers
#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace processor {

/**
 * @class ProcessorPortStub
 * @brief Structure to hold data to build a Processor port.
 * 
 */
class ProcessorPortStub : public swatch::core::AbstractStub {
public:

  ProcessorPortStub(const std::string& aId) : swatch::core::AbstractStub(aId) { }

  virtual ~ProcessorPortStub() { }

  //! Port number
  uint32_t number;
};


//typedef xdata::Bag<ProcessorPortStub> ProcessorPortBag;

/**
 * @class ProcessorStub
 * @brief Struct to hold the data to construct a processor
 */

class ProcessorStub : public swatch::core::AbstractStub {
public:

  ProcessorStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~ProcessorStub() { }

  //! Type of the Processor
  std::string hwtype;

  //! Class to create the Processor object
  std::string creator;

  //! Id of the uTCA crate where the Processor is installed 
  std::string crate;

  //! Slot where the board is installed 
  uint32_t slot;

  //! Uri to access the hardware resource
  std::string uri;

  //! Address table
  std::string addressTable;

  //! List of receiver port bags
  std::vector<ProcessorPortStub> rxPorts;

  //! List of transmitter port bags
  std::vector<ProcessorPortStub> txPorts;

};

std::ostream& operator<<(std::ostream& os, const swatch::processor::ProcessorStub& pd);


} // namespace processor
} // namespace swatch


#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */