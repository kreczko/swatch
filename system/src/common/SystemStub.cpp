/**
 * @file    SystemStub.cpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#include "swatch/system/SystemStub.hpp"


#include <ostream>


namespace swatch {
namespace system {

  
std::ostream& operator<<(std::ostream& os, const swatch::system::SystemStub& sd ) {

  os << "SystemStub[" << sd.id << "]:\n"
//       << "   - Location = " << sd.location << "\n"
//       << "   - Description = " << sd.description << "\n"
     << std::flush;

  return os;
}


} // namespace system
} // namespace swatch

