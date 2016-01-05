/**
 * @file    SystemStub.cpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#include "swatch/system/SystemStub.hpp"


#include <ostream>


namespace swatch {
namespace system {

  
std::ostream& operator<<(std::ostream& aStream, const swatch::system::SystemStub& aStub ) {

  aStream << "SystemStub[" << aStub.id << "]:\n"
//       << "   - Location = " << sd.location << "\n"
//       << "   - Description = " << sd.description << "\n"
     << std::flush;

  return aStream;
}


} // namespace system
} // namespace swatch

