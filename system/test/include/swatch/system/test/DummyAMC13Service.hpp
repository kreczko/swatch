/* 
 * File:   DummyAMC13Service.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 9:18 PM
 */

#ifndef __SWATCH_SYSTEM_DUMMYAMC13SERVICE_HPP__
#define	__SWATCH_SYSTEM_DUMMYAMC13SERVICE_HPP__

#include "swatch/system/DaqTTCService.hpp"

namespace swatch {
namespace system {
namespace test {

class DummyAMC13Service : public system::DaqTTCService {
public:
    DummyAMC13Service( const std::string& aId, const core::XParameterSet& aPars );
    virtual ~DummyAMC13Service();

    virtual uint32_t getSlot() const;

    virtual const std::string& getCrateId() const;
    
    virtual void reset();
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots );
    

  virtual void configureClock(const std::string& mode);


private:

    uint32_t slot_;
    
    std::string crate_;

};
} // namespace test
} // namespace system
} // namespace swatch


#endif	/* __SWATCH_SYSTEM_DUMMYAMC13SERVICE_HPP__ */

