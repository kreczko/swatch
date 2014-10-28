#ifndef SWATCH_PROCESSOR_TTCINTERFACE_HPP
#define SWATCH_PROCESSOR_TTCINTERFACE_HPP

// SWATCH HEADERS

// OTHER HEADERS

// C++ HEADERS
#include <stdint.h>

namespace swatch {
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class TTCInterface {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    TTCInterface()  {}
public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~TTCInterface() {}
    
    // Clear all counters
    virtual void clearCounters() = 0;
    
    // Clear error counters
    virtual void clearErrors() = 0;

    // Spy on the incoming TTC commands
    virtual void spy() = 0;
    
    // TTC Spy
    virtual void maskBC0Spy(bool mask = true) = 0;

    // Send specific BGO command throught the TTC logic
    virtual void sendBGo(uint32_t command) = 0;

    //
    virtual bool isBC0SpyMasked() const = 0;

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

    /**
     * @brief Single bit error counter getter.
     * @return Current value of the single bit error counter.
     */
    virtual uint32_t getSingleBitErrors() const = 0;

    /**
     * @brief Double bit error counter getter.
     * @return Current value of the double bit error counter.
     */
    virtual uint32_t getDoubleBitErrors() const = 0;
    
    virtual bool isClock40Locked() const = 0;
    virtual bool hasClock40Stopped() const = 0;
    virtual bool isOrbitLocked() const = 0;
    virtual bool hasBC0Stopped() const = 0;

};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TTCINTERFACE_HPP */
