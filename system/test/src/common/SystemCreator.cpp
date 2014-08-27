#include "swatch/system/test/SystemCreator.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// uHAL Headers
#include "uhal/log/log.hpp"

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/AMC13Service.hpp"
#include "swatch/system/ServiceFactory.hpp"

namespace swatch {
namespace system {
namespace test {

SWATCH_SYSTEM_REGISTER_CREATOR(SystemCreator);

swatch::system::System*
SystemCreator::operator()(const std::string& aId, const swatch::core::ParameterSet& aPSet) {
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::processor;   

    using namespace uhal;
    
    log(Info(),"Building System ",aId);
    System* sys = new System(aId, aPSet);
    
    std::vector<ParameterSet> vPSets;
    
    log(Info(), "Building Processors");
    vPSets = aPSet.get< std::vector<ParameterSet> >("processors");
    log(Debug(), " + Found ",uhal::Integer(vPSets.size())," processors");
    
    BOOST_FOREACH(ParameterSet& ps,vPSets) {
        log(Debug(),"name: ", ps.get<std::string>("PROCESSOR NAME")," ", ps.get<std::string>("PROCESSOR CREATOR"));
        
        Processor* p = ProcessorFactory::get()->make(ps.get<std::string>("PROCESSOR CREATOR"), ps.get<std::string>("PROCESSOR NAME"), ps);

        sys->add(p);  
    }

    log(Info(), "Building Amc13s");
    vPSets = aPSet.get< std::vector<ParameterSet> >("amc13s");
    log(Debug(), " + Found ",uhal::Integer(vPSets.size())," amc13s");

    BOOST_FOREACH(ParameterSet& ps,vPSets) {
        log(Debug(),"name: ", ps.get<std::string>("SERVICE NAME")," ", ps.get<std::string>("SERVICE CREATOR"));
        
        AMC13Service* a = static_cast<AMC13Service*>(ServiceFactory::get()->make(ps.get<std::string>("SERVICE CREATOR"), ps.get<std::string>("SERVICE NAME"), ps));
        
        sys->add(a);
    }
    return sys;
}
} // namespace test
} // namespace system
} // namespace swatch
