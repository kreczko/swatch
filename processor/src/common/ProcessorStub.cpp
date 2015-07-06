/**
 * @file    ProcessorDescriptor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#include "swatch/processor/ProcessorStub.hpp"

// XDAQ Headers
#include "swatch/core/xoperators.hpp"

namespace swatch {
namespace processor {

std::ostream& operator<<(std::ostream& os, const swatch::processor::ProcessorStub& pd ) {
    
    os << "ProcessorDescriptor[" << pd.id << "]:\n"
       << "   - User creator = " << pd.creator << "\n"
       << "   - Hardware type = " << pd.hwtype << "\n"
       << "   - Uri = " << pd.uri << "\n"
       << "   - Address table = " << pd.addressTable << "\n"
       << "   - Crate ID = " << pd.crate << "\n"
       << "   - Crate Slot = " << pd.slot 
       << std::flush;

    return os;
}

} // namespace processor
} // namespace swatch
