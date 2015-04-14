/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/processor/Utilities.hpp"

// Swatch Headers
#include "swatch/processor/ProcessorStub.hpp"



namespace swatch {
namespace processor {

swatch::core::XParameterSet
treeToProcessorPars(const boost::property_tree::ptree& t) {

    swatch::core::XParameterSet procSet;
    // Fill the stub first (faster)
    ProcessorStub pstub;
    pstub.name         = t.get<std::string>("PROCESSOR NAME");
    pstub.creator      = t.get<std::string>("PROCESSOR CREATOR");
    pstub.uri          = t.get<std::string>("URI");
    pstub.addressTable = t.get<std::string>("ADDRESS TABLE");
    pstub.crate        = t.get<std::string>("CRATE NAME");
    pstub.slot         = t.get<uint32_t>("CRATE SLOT");

    // Then make a bag
    ProcessorBag pbag;
    pbag.bag = pstub;

    // Store the bag in the set
    procSet.add("name", pbag.bag.name);
    procSet.add("class", pbag.bag.creator);
    procSet.add("stub", pbag);
    return procSet;
}


} // namespace processor
} // namespace swatch
