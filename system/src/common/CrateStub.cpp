/**
 * @file    CrateStub.cpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#include "swatch/system/CrateStub.hpp"


#include <ostream>


namespace swatch {
namespace system {

  
std::ostream& operator<<(std::ostream& aStream, const swatch::system::CrateStub& aStub)
{
  aStream << "CrateStub[" << aStub.id << "]:\n"
          << "   - Location = " << aStub.location << "\n"
          << "   - Description = " << aStub.description << "\n"
          << std::flush;

    return aStream;
}


} // namespace system
} // namespace swatch

