/**
 * @file    AMC13ServiceDescriptor.cpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#include "swatch/system/DaqTTCStub.hpp"


#include <ostream>


namespace swatch {
namespace system {


std::ostream& operator<<(std::ostream& os, const swatch::system::DaqTTCStub& sd ) {
    
    os << "DaqTTCStub[" << sd.id << "]:\n"
       << "   - Uses creator = " << sd.creator << "\n"
       << "   - Uri [T1] = " << sd.uriT1 << "\n"
       << "   - Address Table [T1] = " << sd.addressTableT1 << "\n"
       << "   - Uri [T2] = " << sd.uriT2 << "\n"
       << "   - Address Table [T2] = " << sd.addressTableT2 << "\n"
       << "   - Crate ID = " << sd.crate << "\n"
       << "   - Crate Slot = " << sd.slot 
       << "   - FED ID = " << sd.fedId << "\n"
       << std::flush;

    return os;
}


} // namespace system
} // namespace swatch
