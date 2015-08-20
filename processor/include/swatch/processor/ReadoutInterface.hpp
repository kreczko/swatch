
#ifndef SWATCH_PROCESSOR_READOUTINTERFACE_HPP
#define SWATCH_PROCESSOR_READOUTINTERFACE_HPP


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

//! Abstract class defining the readout component interface of a processor
class ReadoutInterface: public core::MonitorableObject {
protected:

  ReadoutInterface();

public:

  virtual ~ReadoutInterface();

  static const std::vector<std::string>  defaultMetrics;
};


} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_READOUTINTERFACE_HPP */
