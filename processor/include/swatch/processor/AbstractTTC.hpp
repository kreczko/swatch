#ifndef SWATCH_PROCESSOR_ABSTRACTTTC_HPP
#define SWATCH_PROCESSOR_ABSTRACTTTC_HPP
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Component.hpp"

// C++ HEADERS
#include <stdint.h>

namespace swatch {
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractTTC : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractTTC(Connection* connection) : Component(connection)  {}
public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractTTC() {}

    /**
     * Set of valid configurations
     * @return set of configurations 
     */
    virtual std::set<std::string> configurations() const = 0; 
    
    /**
     * 
     * @param config
     */
    virtual void configure(const std::string& config) = 0;
    /**
     * Enable external TTC inputs
     * @param enable: enables/disables external TTC inputs
     */
    virtual void enable(bool enable = true) = 0;
    
    /**
     * Generate orbit signal internally
     * @param generate: enables/disables orbit generation
     */
    virtual void generateInternalOrbit(bool generate = true) = 0;
    
    /**
     * Send a single L1A (parameters required) 
     */
    virtual void sendSingleL1A() = 0;
    
    // Send Multiple L1As
    virtual void sendMultipleL1A() = 0;
    
    // Clear all counters
    virtual void clearCounters() = 0;
    
    // Clear error counters
    virtual void clearErrCounters() = 0;

    // Spy on the incoming TTC commands
    virtual void spy() = 0;
    
    // TTC Spy
    virtual void maskBC0Spy(bool mask = true) = 0;

    // Send specific BGO command throught the TTC logic
    virtual void sendBGo(uint32_t command) = 0;

    // 
    virtual bool isEnabled() const = 0;
    
    //
    virtual bool isGeneratingInternalBC0() const = 0;

    //
    virtual bool isBC0SpyMasked() const = 0;

    //monitoring
    virtual uint32_t getBunchCount() const = 0;
    virtual uint32_t getEvtCount() const = 0;
    virtual uint32_t getOrbitCount() const = 0;
    virtual uint32_t getSingleBitErrorCounter() const = 0;
    virtual uint32_t getDoubleBitErrorCounter() const = 0;
    //virtual void getTTChistory() const   = 0;
    
    virtual bool isClock40Locked() const = 0;
    virtual bool hasClock40Stopped() const = 0;
    virtual bool isBC0Locked() const = 0;
    virtual bool hasBC0Stopped() const = 0;

};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_ABSTRACTTTC_HPP */
