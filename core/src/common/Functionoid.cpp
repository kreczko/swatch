/*
 * Functionoid.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: arose
 */
#include <iostream>
#include "swatch/core/Functionoid.hpp"

#include <boost/foreach.hpp>


//using namespace swatch::logger;
namespace swatch {
namespace core {
Functionoid::Functionoid( const std::string& aId ) :
        Object( aId )
{
}

Functionoid::~Functionoid() {
  // TODO Auto-generated destructor stub
}


Object* Functionoid::getParent( const uint32_t& aDepth ) {
  return getAncestor(aDepth);
}



} /* namespace core */
} /* namespace swatch */
