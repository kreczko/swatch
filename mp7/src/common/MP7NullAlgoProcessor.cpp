
#include "swatch/mp7/MP7NullAlgoProcessor.hpp"


// SWATCH headers
#include "swatch/core/Factory.hpp"

// SWATCH mp7 headers
#include "swatch/mp7/MP7NullAlgo.hpp"


SWATCH_REGISTER_CLASS(swatch::mp7::MP7NullAlgoProcessor);


namespace swatch {
namespace mp7 {


MP7NullAlgoProcessor::MP7NullAlgoProcessor(const std::string& id, const swatch::core::XParameterSet& aPars) :
    MP7Processor(id, aPars)
{
  //! Add algo interface
  Add( new MP7NullAlgo(this->driver()) );
}


MP7NullAlgoProcessor::~MP7NullAlgoProcessor() {
}


} // namespace mp7
} // namespace swatch
