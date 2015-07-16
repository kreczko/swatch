/* 
 * File:   Link.cpp
 * Author: ale
 */

#include "swatch/processor/Link.hpp" 


#include "swatch/processor/Port.hpp"


namespace swatch {
namespace processor {

Link::Link(const std::string& aId, OutputPort* aSrc, InputPort* aDst) :
    ObjectView( aId ), 
    src_(aSrc), 
    dst_(aDst)
{
    this->addObj(src_, "src");
    this->addObj(dst_, "dst");
}

Link::~Link() {   
}

} // namespace core
} // namespace swatch
