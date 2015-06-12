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

//! @FIXME A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class TTCInterface: public core::MonitorableObject {
protected:

    TTCInterface();

public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~TTCInterface() {}
    
    // Clear all counters
    virtual void clearCounters() = 0;
    
    // Clear error counters
    virtual void clearErrors() = 0;

    /**
     * @brief Bunch counter getter.
     * @return Current value of the bunch counter.
     */
    virtual uint32_t getBunchCounter() const = 0;

    /**
     * @brief Event counter getter.
     * @return Current value of the event counter.
     */
    virtual uint32_t getEventCounter() const = 0;

    /**
     * @brief Orbit counter getter.
     * @return Current value of the orbit counter.
     */
    virtual uint32_t getOrbitCounter() const = 0;

protected:
    //! Metric indicating if clk40 is locked
    core::Metric<bool>& metricIsClock40Locked_;
    //! Metric indicating if clk40 has stopped
    core::Metric<bool>& metricHasClock40Stopped_;
    //! Metric indicating if BC0 is locked
    core::Metric<bool>& metricIsBC0Locked_;
    //! Metric representing single-bit error counter
    core::Metric<uint32_t>& metricSingleBitErrors_;
    //! Metric representing double-bit error counter
    core::Metric<uint32_t>& metricDoubleBitErrors_;
    
public:
    static std::vector<std::string> getDefaultMetrics();
    
};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TTCINTERFACE_HPP */
