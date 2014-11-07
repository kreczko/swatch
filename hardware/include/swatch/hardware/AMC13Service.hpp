/**
 * @file    AMC13Service.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */


#ifndef SWATCH_HARDWARE_AMC13SERVICE_HPP
#define SWATCH_HARDWARE_AMC13SERVICE_HPP

// Swatch Headers

#include "swatch/system/AMC13Service.hpp"

namespace swatch {
namespace hardware {

class AMC13Service : swatch::system::AMC13Service {
public:
    AMC13Service( const std::string& aId, const core::ParameterSet& params );
    ~AMC13Service();

    virtual void enableTTC(const std::vector<uint32_t>& aSlots);

};

} // namespace hardware
} // namespace swatch

#endif /* SWATCH_HARDWARE_AMC13SERVICE_HPP */
