#include "swatch/system/SystemLoggingCreator.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/DaqTTCService.hpp"
#include "swatch/system/ServiceFactory.hpp"

namespace swlog = swatch::logger;
namespace swco = swatch::core;
namespace swsys = swatch::system;
namespace swpro = swatch::processor;

namespace swatch {
namespace system {

SWATCH_SYSTEM_REGISTER_CREATOR(SystemLoggingCreator);

swatch::system::System*
SystemLoggingCreator::operator()(const std::string& aId, const swatch::core::XParameterSet& aPars) {
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building System " << aId;
    System* sys = createSystem(aId, aPars);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Crates";
    addCrates(sys, aPars);
    
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Processors";
    addProcessors(sys, aPars);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building DaqTTC";
    addDaqTTC(sys, aPars);
    
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Services";
    addServices(sys, aPars);

    return sys;
}
} // namespace system
} // namespace swatch
