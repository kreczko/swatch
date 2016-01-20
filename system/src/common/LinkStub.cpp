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
       << "   - source port = " << aStub.src << "\n"
       << "   - destination port = " << aStub.dst << "\n"
       << std::flush;

    return aStream;
}

} // namespace processor
} // namespace swatch
