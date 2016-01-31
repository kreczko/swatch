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


std::ostream& operator<<(std::ostream& aStream, const swatch::dtm::DaqTTCStub& aStub )
{
    aStream << "DaqTTCStub[" << aStub.id << "]:\n"
       << "   - Uses creator = " << aStub.creator << "\n"
       << "   - Uri [T1] = " << aStub.uriT1 << "\n"
       << "   - Address Table [T1] = " << aStub.addressTableT1 << "\n"
       << "   - Uri [T2] = " << aStub.uriT2 << "\n"
       << "   - Address Table [T2] = " << aStub.addressTableT2 << "\n"
       << "   - Crate ID = " << aStub.crate << "\n"
       << "   - Crate Slot = " << aStub.slot 
       << "   - FED ID = " << aStub.fedId << "\n"
       << std::flush;

    return aStream;
}


} // namespace dtm
} // namespace swatch
