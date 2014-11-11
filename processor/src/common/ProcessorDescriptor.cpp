/**
 * @file    ProcessorDescriptor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */


#include "swatch/processor/ProcessorDescriptor.hpp"


namespace swatch {
namespace processor {

std::ostream& operator<<(std::ostream& os, const ProcessorDescriptor& pd ) {
    
    os << "ProcessorDescriptor[" << pd.name << "]:\n"
       << "   - Uses creator = " << pd.creator << "\n"
       << "   - Uri = " << pd.uri << "\n"
       << "   - Address Table = " << pd.addressTable << "\n"
       << "   - Crate ID = " << pd.crateId << "\n"
       << "   - Crate Slot = " << pd.slot 
       << std::flush;

    return os;
}

} // namespace processor
} // namespace swatch
