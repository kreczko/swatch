
#include "swatch/processor/ProcessorStub.hpp"


// Standard headers
#include <ostream>

// SWATCH headers
#include "swatch/processor/Processor.hpp"


namespace swatch {
namespace processor {

ProcessorStub::ProcessorStub(const std::string& aId) :
  AbstractStub(aId),
  slot(Processor::kNoSlot)
{ }
  
std::ostream& operator<<(std::ostream& aStream, const swatch::processor::ProcessorStub& aStub )
{
    aStream << "ProcessorDescriptor[" << aStub.id << "]:\n"
       << "   - User creator = " << aStub.creator << "\n"
       << "   - Hardware type = " << aStub.hwtype << "\n"
       << "   - Role = " << aStub.role << "\n"
       << "   - Uri = " << aStub.uri << "\n"
       << "   - Address table = " << aStub.addressTable << "\n"
       << "   - Crate ID = " << aStub.crate << "\n"
       << "   - Crate Slot = " << aStub.slot 
       << std::flush;

    return aStream;
}

} // namespace processor
} // namespace swatch
