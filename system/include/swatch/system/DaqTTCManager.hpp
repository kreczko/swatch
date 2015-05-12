/* 
 * @file    DaqTTCManager.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 *
 */

#ifndef __SWATCH_SYSTEM_DAQTTCMANAGER_HPP__
#define	__SWATCH_SYSTEM_DAQTTCMANAGER_HPP__

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/ActionableObject.hpp"

#include <string>

namespace swatch {
namespace system {

class DaqTTCManager : public core::ActionableObject {
protected:
    explicit DaqTTCManager( const std::string& aId, const core::XParameterSet& params );
public:
    virtual ~DaqTTCManager();
    
    virtual uint32_t getSlot() const = 0;

    virtual const std::string& getCrateId() const = 0;

    virtual void reset() = 0;
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots ) = 0;

    virtual void configureClock(const std::string& mode) = 0;

    /// Reads measured TTC clock frequency (in Hz) 
    virtual double ttcClockFreq() const = 0;
    
    /// Reads TTC BC0 counter
    virtual uint32_t ttcBC0Counter() const = 0;
    
    /// Reads TTC BC error counter
    virtual uint32_t ttcBC0Errors() const = 0;
    
    /// Reads TTC single-bit error counter
    virtual uint32_t ttcSingleBitErrors() const = 0;
    
    /// Reads TTC double-bit error counter
    virtual uint32_t ttcDoubleBitErrors() const = 0;
};

} // namespace system
} // namespace swatch



#endif	/* __SWATCH_SYSTEM_DAQTTCSERVICE_HPP__ */

