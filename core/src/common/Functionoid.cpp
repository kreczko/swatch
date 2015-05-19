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
        Object( aId ) {
}

Functionoid::~Functionoid() {
  // TODO Auto-generated destructor stub
}

const XParameterSet&
Functionoid::getDefaultParams() const {
  return parameters_;
}

Object* Functionoid::getParent( const uint32_t& aDepth ) {
  return getAncestor(aDepth);
}

const XParameterSet Functionoid::mergeParametersWithDefaults(
    const XParameterSet& params) const {
  XParameterSet merged = XParameterSet(params);
  merged.update(parameters_);
  return merged;
}

} /* namespace core */
} /* namespace swatch */
