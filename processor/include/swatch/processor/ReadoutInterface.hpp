
#ifndef __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__
#define __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"
#include "swatch/core/TTSUtils.hpp"


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
  core::Metric<core::tts::State>& mMetricTTS;
  core::Metric<bool>& mMetricAMCCoreReady;
  core::Metric<uint32_t>& mMetricEventCounter;
};


} // namespace processor
} // namespace swatch

#endif /* __SWATCH_PROCESSOR_READOUTINTERFACE_HPP__ */
