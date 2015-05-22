/**
 * @file    AMC13ServiceDescriptor.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */


#include "swatch/system/DaqTTCStub.hpp"

// Swatch Headers
#include "swatch/core/xoperators.hpp"

namespace swatch {
namespace system {

} // namespace system
} // namespace swatch


std::ostream& operator<<(std::ostream& os, const swatch::system::DaqTTCStub& sd ) {
    
    os << "DaqTTCStub[" << sd.name << "]:\n"
       << "   - Uses creator = " << sd.creator << "\n"
       << "   - Uri [T1] = " << sd.uriT1 << "\n"
       << "   - Address Table [T1] = " << sd.addressTableT1 << "\n"
       << "   - Uri [T2] = " << sd.uriT2 << "\n"
       << "   - Address Table [T2] = " << sd.addressTableT2 << "\n"
       << "   - Crate ID = " << sd.crate << "\n"
       << "   - Crate Slot = " << sd.slot 
       << std::flush;

    return os;
}
