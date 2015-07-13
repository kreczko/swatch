/*
 * Functionoid.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: arose
 */

#include "swatch/core/Functionoid.hpp"


namespace swatch {
namespace core {

Functionoid::Functionoid( const std::string& aId ) :
  Object( aId )
{
}


Functionoid::~Functionoid() {
}


Object* Functionoid::getParent( const uint32_t& aDepth ) {
  return getAncestor(aDepth);
}


} /* namespace core */
} /* namespace swatch */
