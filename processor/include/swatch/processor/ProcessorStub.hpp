/**
 * @file    ProcessorStub.hpp
 * @author  Alessandro Thea
 * @date    09/11/14
 */

#ifndef __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__
#define __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__


// C++ headers
#include <iosfwd>
#include <vector>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"


namespace swatch {
namespace processor {

//! Structure to hold data to build a Processor port.
class ProcessorPortStub : public swatch::core::AbstractStub {
public:

  ProcessorPortStub(const std::string& aId) : swatch::core::AbstractStub(aId) { }

  virtual ~ProcessorPortStub() { }

  //! Port number
  uint32_t number;
};


//! Structure that holds the data required to construct a processor
class ProcessorStub : public swatch::core::AbstractStub {
public:

  ProcessorStub(const std::string& aId);

  virtual ~ProcessorStub() { }

  //! Class to create the Processor object
  std::string creator;

  //! Type of the Processor
  std::string hwtype;

  //! Role of the processor
  std::string role;
  
  //! Uri to access the hardware resource
  std::string uri;

  //! Address table
  std::string addressTable;

  //! Id of the uTCA crate where the Processor is installed 
  std::string crate;

  //! Slot where the board is installed 
  uint32_t slot;

  //! List of receiver port bags
  std::vector<ProcessorPortStub> rxPorts;

  //! List of transmitter port bags
  std::vector<ProcessorPortStub> txPorts;
};

std::ostream& operator<<(std::ostream& os, const swatch::processor::ProcessorStub& pd);


} // namespace processor
} // namespace swatch


#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */