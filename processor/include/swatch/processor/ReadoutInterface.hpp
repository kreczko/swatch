
#ifndef __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__
#define __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__


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

  static const std::vector<std::string>  kDefaultMetrics;

protected:
  core::Metric<std::string>& mMetricTTS;
  core::Metric<bool>& mMetricAMCCoreReady;
};


} // namespace processor
} // namespace swatch

#endif /* __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__ */
