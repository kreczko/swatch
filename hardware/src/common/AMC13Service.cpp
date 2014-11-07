/**
 * @file    AMC13Service.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

 #include "swatch/hardware/AMC13Service.hpp"


namespace swatch {
namespace hardware {

AMC13Service::AMC13Service(const std::string& aId, const core::ParameterSet& params) :
swatch::system::AMC13Service(aId, params) {

}

AMC13Service::~AMC13Service() {

}

void AMC13Service::enableTTC(const std::vector<uint32_t>& aSlots) {

}

} // namespace hardware
} // namespace swatch