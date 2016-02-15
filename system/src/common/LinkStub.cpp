/**
 * @file    LinkStub.cpp
 * @author  Tom Williams
 * @date    June 2015
 */

#include "swatch/system/LinkStub.hpp"


#include <ostream>


namespace swatch {
namespace system {

std::ostream& operator<<(std::ostream& aStream, const swatch::system::LinkStub& aStub)
{    
    aStream << "LinkStub[" << aStub.id << "]:\n"
       << "   - source processor = " << aStub.srcProcessor << ", port = " << aStub.srcPort << "\n"
       << "   - destination processor = " << aStub.dstProcessor << ", port = " << aStub.dstPort << "\n"
       << std::flush;

    return aStream;
}

} // namespace processor
} // namespace swatch
