/**
 * @file    CrateStub.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */


#include "swatch/system/CrateStub.hpp"

namespace swatch {
namespace system {

  
std::ostream& operator<<(std::ostream& os, const swatch::system::CrateStub& sd ) {
    
    os << "CrateStub[" << sd.id << "]:\n"
       << "   - Location = " << sd.location << "\n"
       << "   - Description = " << sd.description << "\n"
       << std::flush;

    return os;
}


} // namespace system
} // namespace swatch

