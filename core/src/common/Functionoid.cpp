
#include "swatch/core/Functionoid.hpp"


namespace swatch {
namespace core {

Functionoid::Functionoid( const std::string& aId ) :
  Object( aId )
{
}


Functionoid::~Functionoid() {
}


const Object* Functionoid::getParent() const {
  return getAncestor(1);
}

Object* Functionoid::getParent() {
  return getAncestor(1);
}




} /* namespace core */
} /* namespace swatch */
