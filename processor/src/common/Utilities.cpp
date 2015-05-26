/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/processor/Utilities.hpp"

// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/processor/ProcessorStub.hpp"



namespace swatch {
namespace processor {

swatch::core::XParameterSet
treeToProcessorPars(const boost::property_tree::ptree& t) {

    // Fill the stub with basic info
    ProcessorStub pstub;
    pstub.name         = t.get<std::string>("PROCESSOR NAME");
    pstub.hwtype       = t.get<std::string>("HARDWARE TYPE");
    pstub.creator      = t.get<std::string>("PROCESSOR CREATOR");
    pstub.uri          = t.get<std::string>("URI");
    pstub.addressTable = t.get<std::string>("ADDRESS TABLE");
    pstub.crate        = t.get<std::string>("CRATE NAME");
    pstub.slot         = t.get<uint32_t>("CRATE SLOT");

    // Iterate over rx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& rxPortInfo, t.get_child("RX PORTS"))
    {
      xdata::Bag<ProcessorPortStub> b;
      b.bag.name = rxPortInfo.second.get<std::string>("NAME");
      b.bag.number = rxPortInfo.second.get<std::string>("PID");
      pstub.rxPorts.push_back(b);
    }
    
    // Iterate over tx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& txPortInfo, t.get_child("TX PORTS"))
    {
      xdata::Bag<ProcessorPortStub> b;
      b.bag.name = txPortInfo.second.get<std::string>("NAME");
      b.bag.number = txPortInfo.second.get<std::string>("PID");
      pstub.txPorts.push_back(b);
    }

    
    // Finally, make the processor bag
    ProcessorBag pbag;
    pbag.bag = pstub;

    // Store the bag in the XParameterSet
    swatch::core::XParameterSet procSet;
    procSet.add("name", pbag.bag.name);
    procSet.add("class", pbag.bag.creator);
    procSet.add("stub", pbag);
    
    return procSet;
}


} // namespace processor
} // namespace swatch
