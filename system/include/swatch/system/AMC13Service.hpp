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
public:
    explicit AMC13Service( const std::string& aId, const core::Arguments& aArguments );
    virtual ~AMC13Service();
    
    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots ) = 0;
    
protected:
    uint32_t slot_;
    std::string crateId_;
};

} // namespace system
} // namespace swatch



#endif	/* __swatch_system_amc13service_hpp__ */

