
#include "swatch/mp7/MP7ReadoutInterface.hpp"



namespace swatch {
namespace mp7 {

MP7ReadoutInterface::MP7ReadoutInterface( ::mp7::MP7Controller& controller ) :
  driver_(controller)
{
}


MP7ReadoutInterface::~MP7ReadoutInterface() {

}


void MP7ReadoutInterface::implementUpdateMetrics()
{
}

} // namespace mp7
} // namespace swatch
