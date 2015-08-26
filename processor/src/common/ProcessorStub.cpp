
#include "swatch/processor/ProcessorStub.hpp"


// Standard headers
#include <ostream>

// SWATCH headers
#include "swatch/processor/Processor.hpp"


namespace swatch {
namespace processor {

ProcessorStub::ProcessorStub(const std::string& aId) :
  AbstractStub(aId),
  slot(Processor::NoSlot)
{ }
  
std::ostream& operator<<(std::ostream& os, const swatch::processor::ProcessorStub& pd ) {
    
    os << "ProcessorDescriptor[" << pd.id << "]:\n"
       << "   - User creator = " << pd.creator << "\n"
       << "   - Hardware type = " << pd.hwtype << "\n"
       << "   - Role = " << pd.role << "\n"
       << "   - Uri = " << pd.uri << "\n"
       << "   - Address table = " << pd.addressTable << "\n"
       << "   - Crate ID = " << pd.crate << "\n"
       << "   - Crate Slot = " << pd.slot 
       << std::flush;

    return os;
}

} // namespace processor
} // namespace swatch
