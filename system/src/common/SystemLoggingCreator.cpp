#include "swatch/system/SystemLoggingCreator.hpp"


// boost headers
#include <boost/foreach.hpp>

// SWATCH headers
#include "swatch/system/System.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"


namespace swlog = swatch::logger;
namespace swco = swatch::core;
namespace swsys = swatch::system;
namespace swpro = swatch::processor;


//SWATCH_REGISTER_CREATOR(swatch::system::SystemLoggingCreator);


namespace swatch {
namespace system {

/* 
swatch::core::Object*
SystemLoggingCreator::operator()(const swatch::core::AbstractStub& aStub ) {
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building System " << aStub.id;
    System* sys = createSystem(aStub);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Crates";
    addCrates(sys);
    
    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Processors";
    addProcessors(sys);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building DaqTTC";
    addDaqTTCs(sys);
    
//    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Services";
//    addServices(sys);

    LOG(swlog::kNotice) << "SystemLoggingCreator: Building Links";
    addLinks(sys);
    
    sys->registerActions();
    
    return sys;
}
*/


} // namespace system
} // namespace swatch
