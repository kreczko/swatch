/**
 * @file    CrateStub.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */


#include "swatch/system/CrateStub.hpp"

// XDAQ Headers
#include "swatch/core/xoperators.hpp"

namespace swatch {
namespace system {
} // namespace system
} // namespace swatch

std::ostream& operator<<(std::ostream& os, const swatch::system::CrateStub& sd ) {
    
    os << "CrateStub[" << sd.name << "]:\n"
       << "   - Location = " << sd.location << "\n"
       << "   - Description = " << sd.description << "\n"
       << std::flush;

    return os;
}


