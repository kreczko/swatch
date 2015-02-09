/* 
 * File:   AMC13.hpp
 * Author: ale
 *
 * Created on July 28, 2014, 12:13 AM
 */

#ifndef SWATCH_SYSTEM_DAQTTCSERVICE_HPP
#define	SWATCH_SYSTEM_DAQTTCSERVICE_HPP

// Swatch Headers
#include "swatch/system/Service.hpp"

#include <string>

namespace swatch {
namespace system {

class DaqTTCService : public Service {
protected:
    explicit DaqTTCService( const std::string& aId, const core::ParameterSet& params );
public:
    virtual ~DaqTTCService();
    
    virtual uint32_t getSlot() const = 0;

    virtual const std::string& getCrateId() const = 0;
    
    /// List the operation modes available
    virtual std::set<std::string> getModes() const = 0;

    ///
    virtual void reset( const std::string& mode ) = 0;

    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots ) = 0;
    
};

} // namespace system
} // namespace swatch



#endif	/* SWATCH_SYSTEM_DAQTTCSERVICE_HPP */

