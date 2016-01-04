/**
 * @file    LinkStub.cpp
 * @author  Tom Williams
 * @date    June 2015
 */

#include "swatch/processor/LinkStub.hpp"


#include <ostream>


namespace swatch {
namespace processor {

std::ostream& operator<<(std::ostream& aStream, const swatch::processor::LinkStub& aStub)
{    
    aStream << "LinkStub[" << aStub.id << "]:\n"
       << "   - source port = " << aStub.src << "\n"
       << "   - destination port = " << aStub.dst << "\n"
       << std::flush;

    return aStream;
}

} // namespace processor
} // namespace swatch
