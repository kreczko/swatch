/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/processor/Utilities.hpp"

#include "swatch/processor/ProcessorStub.hpp"


namespace swatch {
namespace processor {

swatch::core::ParameterSet
treeToProcessorPars(const boost::property_tree::ptree& t) {

    swatch::core::ParameterSet procSet;
    swatch::processor::ProcessorStub pd;
    pd.name         = t.get<std::string>("PROCESSOR NAME");
    pd.creator      = t.get<std::string>("PROCESSOR CREATOR");
    pd.uri          = t.get<std::string>("URI");
    pd.addressTable = t.get<std::string>("ADDRESS TABLE");
    pd.crate      = t.get<std::string>("CRATE NAME");
    pd.slot         = t.get<uint32_t>("CRATE SLOT");

    procSet.set("name", pd.name);
    procSet.set("class", pd.creator);
    procSet.set("descriptor", pd);
    return procSet;
}


} // namespace processor
} // namespace swatch
