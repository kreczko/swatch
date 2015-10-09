/**
 * @file    AMC13ServiceDescriptor.cpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/logger/Log.hpp"


#include <ostream>


namespace swatch {
namespace dtm {


std::ostream& operator<<(std::ostream& os, const swatch::dtm::DaqTTCStub& sd ) {
    
    os << "DaqTTCStub[" << sd.id << "]:\n"
       << "   - Uses creator = " << sd.creator << "\n"
       << "   - Uri [T1] = " << sd.uriT1 << "\n"
       << "   - Address Table [T1] = " << sd.addressTableT1 << "\n"
       << "   - Uri [T2] = " << sd.uriT2 << "\n"
       << "   - Address Table [T2] = " << sd.addressTableT2 << "\n"
       << "   - Crate ID = " << sd.crate << "\n"
       << "   - Crate Slot = " << sd.slot 
       << "   - FED ID = " << sd.fedId << "\n"
       << "   - AMC SLOTS = " << swatch::logger::shortVecFmt(sd.amcSlots)
       << std::flush;

    return os;
}


} // namespace system
} // namespace swatch
