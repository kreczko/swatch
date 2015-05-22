/**
 * @file    ProcessorDescriptor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#include "swatch/processor/ProcessorStub.hpp"

// XDAQ Headers
#include "swatch/core/xoperators.hpp"



std::ostream& operator<<(std::ostream& os, const swatch::processor::ProcessorStub& pd ) {
    
    os << "ProcessorDescriptor[" << pd.name << "]:\n"
       << "   - Uses creator = " << pd.creator << "\n"
       << "   - HardwareType = " << pd.hwtype << "\n"
       << "   - Uri = " << pd.uri << "\n"
       << "   - Address Table = " << pd.addressTable << "\n"
       << "   - Crate ID = " << pd.crate << "\n"
       << "   - Crate Slot = " << pd.slot 
       << std::flush;

    return os;
}

namespace swatch {
namespace processor {

} // namespace processor
} // namespace swatch
