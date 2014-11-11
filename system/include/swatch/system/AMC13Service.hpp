/* 
 * File:   AMC13.hpp
 * Author: ale
 *
 * Created on July 28, 2014, 12:13 AM
 */

#ifndef __swatch_system_amc13service_hpp__
#define	__swatch_system_amc13service_hpp__

// Swatch Headers
#include "swatch/system/Service.hpp"

#include <string>

namespace swatch {
namespace system {

class AMC13Service : public Service {
protected:
    explicit AMC13Service( const std::string& aId, const core::ParameterSet& params );
public:
    virtual ~AMC13Service();
    
    virtual uint32_t getSlot() const = 0;

    virtual const std::string& getCrateId() const = 0;
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots ) = 0;
    
};

} // namespace system
} // namespace swatch



#endif	/* __swatch_system_amc13service_hpp__ */

