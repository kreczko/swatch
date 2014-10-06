#include "swatch/system/SystemLoggingCreator.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// uHAL Headers
#include "uhal/log/log.hpp"

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/AMC13Service.hpp"
#include "swatch/system/ServiceFactory.hpp"
using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::processor;
using namespace uhal;

namespace swatch {
namespace system {

SWATCH_SYSTEM_REGISTER_CREATOR(SystemLoggingCreator);

swatch::system::System*
SystemLoggingCreator::operator()(const std::string& aId, const swatch::core::ParameterSet& params) {
    log(Info(),"Building System ",aId);
    System* sys = create_system(aId, params);
    
    log(Info(), "Building Processors");
    add_processors(sys, params);

    log(Info(), "Building Services");
    add_services(sys, params);

    return sys;
}
} // namespace system
} // namespace swatch
