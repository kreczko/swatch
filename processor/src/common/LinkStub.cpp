/**
 * @file    LinkStub.cpp
 * @author  Tom Williams
 * @date    June 2015
 */

#include "swatch/processor/LinkStub.hpp"


#include <ostream>

#include "swatch/core/xoperators.hpp"


namespace swatch {
namespace processor {

std::ostream& operator<<(std::ostream& os, const swatch::processor::LinkStub& pd ) {
    
    os << "LinkStub[" << pd.id << "]:\n"
       << "   - source port = " << pd.src << "\n"
       << "   - destination port = " << pd.dst << "\n"
       << std::flush;

    return os;
}

} // namespace processor
} // namespace swatch
