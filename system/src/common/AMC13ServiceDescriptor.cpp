/**
 * @file    AMC13ServiceDescriptor.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */


#include "swatch/system/AMC13ServiceDescriptor.hpp"


namespace swatch {
namespace system {

std::ostream& operator<<(std::ostream& os, const AMC13ServiceDescriptor& sd ) {
    
    os << "AMC13ServiceDescriptor[" << sd.name << "]:\n"
       << "   - Uses creator = " << sd.creator << "\n"
       << "   - Uri [T1] = " << sd.t1Uri << "\n"
       << "   - Address Table [T1] = " << sd.t1AddressTable << "\n"
       << "   - Uri [T2] = " << sd.t2Uri << "\n"
       << "   - Address Table [T2] = " << sd.t2AddressTable << "\n"
       << "   - Crate ID = " << sd.crateId << "\n"
       << "   - Crate Slot = " << sd.slot 
       << std::flush;

    return os;
}

} // namespace system
} // namespace swatch
