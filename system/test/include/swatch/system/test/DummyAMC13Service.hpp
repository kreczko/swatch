/* 
 * File:   DummyAMC13Service.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 9:18 PM
 */

#ifndef __swatch_system_dummyamc13service_hpp__
#define	__swatch_system_dummyamc13service_hpp__

#include "swatch/system/AMC13Service.hpp"

namespace swatch {
namespace system {
namespace test {

class DummyAMC13Service : public system::AMC13Service {
public:
    DummyAMC13Service( const std::string& aId, const core::ParameterSet& params );
    virtual ~DummyAMC13Service();
    
    /// Enables ttc commands on the given slots
    virtual void enableTTC( const std::vector<uint32_t> & aSlots );

private:

};
} // namespace test
} // namespace system
} // namespace swatch


#endif	/* __swatch_system_dummyamc13service_hpp__ */

