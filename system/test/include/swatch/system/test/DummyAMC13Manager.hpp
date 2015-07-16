/* 
 * File:   DummyAMC13Service.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 9:18 PM
 */

#ifndef __SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__
#define	__SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__

#include "swatch/system/DaqTTCManager.hpp"

namespace swatch {
namespace system {
namespace test {

class DummyAMC13Manager : public system::DaqTTCManager {
public:
    DummyAMC13Manager( const swatch::core::AbstractStub& aStub );
    virtual ~DummyAMC13Manager();

    virtual void reset();
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots );
    
    virtual void configureClock(const std::string& mode);
    
    /*
        /// Reads measured TTC clock frequency (in Hz) 
    virtual double ttcClockFreq() const;
    
    /// Reads TTC BC0 counter
    virtual uint32_t ttcBC0Counter() const;
    
    /// Reads TTC BC error counter
    virtual uint32_t ttcBC0Errors() const;
    
    /// Reads TTC single-bit error counter
    virtual uint32_t ttcSingleBitErrors() const;
    
    /// Reads TTC double-bit error counter
    virtual uint32_t ttcDoubleBitErrors() const;
    */
    
private:
    virtual void retrieveMetricValues();
};

} // namespace test
} // namespace system
} // namespace swatch


#endif	/* __SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__ */

