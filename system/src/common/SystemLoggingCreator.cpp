#include "swatch/system/SystemLoggingCreator.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/system/System.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/system/DaqTTCManager.hpp"

namespace swlog = swatch::logger;
namespace swco = swatch::core;
namespace swsys = swatch::system;
namespace swpro = swatch::processor;

SWATCH_REGISTER_CREATOR(swatch::system::SystemLoggingCreator);

namespace swatch {
namespace system {

swatch::core::Object*
SystemLoggingCreator::operator()(const std::string& aId, const swatch::core::XParameterSet& aPars) {
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building System " << aId;
    System* sys = createSystem(aId, aPars);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Crates";
    addCrates(sys, aPars);
    
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Processors";
    addProcessors(sys, aPars);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building DaqTTC";
    addDaqTTCs(sys, aPars);
    
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Services";
    addServices(sys, aPars);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Links";
    addLinks(sys, aPars);
    
    return sys;
}
} // namespace system
} // namespace swatch
