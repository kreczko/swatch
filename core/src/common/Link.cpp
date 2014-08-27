/* 
 * File:   Link.cpp
 * Author: ale
 * 
 * Created on July 27, 2014, 10:59 PM
 */

#include "swatch/core/Link.hpp" 

namespace swatch {
namespace core {

Link::Link(const std::string& aId, OutputPort* aSrc, InputPort* aDst, const Arguments& aArguments) :
    ObjectView( aId ), src_(aSrc), dst_(aDst) {
    this->addObj(src_, "src");
    this->addObj(dst_, "dst");
}

Link::~Link() {   
}

} // namespace core
} // namespace swatch