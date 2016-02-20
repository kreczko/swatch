/**
 * @file    TTCInterface.hpp
 * @author  Maxime Guilbaud
 * @brief   Brief description
 * @date    24/07/2014
 */

#ifndef SWATCH_PROCESSOR_TTCINTERFACE_HPP
#define SWATCH_PROCESSOR_TTCINTERFACE_HPP

// SWATCH HEADERS
#include "swatch/core/MonitorableObject.hpp"

// OTHER HEADERS

// C++ HEADERS
#include <stdint.h>

namespace swatch {
namespace processor {

//! Abstract class defining the TTC component interface of a processor
class TTCInterface: public core::MonitorableObject {
protected:
    TTCInterface();

public:
    virtual ~TTCInterface() {}


protected:
    //! Metric containing l1a counter values
    core::Metric<uint32_t>& mMetricL1ACounter;;
    
    //! Metric containing bunch counter value
    core::Metric<uint32_t>& mMetricBunchCounter;
    
    //! Metric containing orbit counter value
    core::Metric<uint32_t>& mMetricOrbitCounter;
    
    //! Metric indicating if clk40 is locked
    core::Metric<bool>& mMetricIsClock40Locked;
    
    //! Metric indicating if clk40 has stopped
    core::Metric<bool>& mMetricHasClock40Stopped;
    
    //! Metric indicating if BC0 is locked
    core::Metric<bool>& mMetricIsBC0Locked;
    
    //! Metric representing single-bit error counter
    core::Metric<uint32_t>& mMetricSingleBitErrors;
    
    //! Metric representing double-bit error counter
    core::Metric<uint32_t>& mMetricDoubleBitErrors;
    
public:
    static const std::vector<std::string> kDefaultMetrics;
    
};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TTCINTERFACE_HPP */
