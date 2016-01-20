/* 
 * File:   Link.cpp
 * Author: ale
 */

#include "swatch/system/Link.hpp" 

#include "swatch/processor/Port.hpp"


namespace swatch {
namespace system {

Link::Link(const std::string& aId, processor::OutputPort* aSrc, processor::InputPort* aDst) :
    ObjectView( aId ), 
    mSrc(aSrc), 
    mDst(aDst)
{
    this->addObj(mSrc, "src");
    this->addObj(mDst, "dst");
}

Link::~Link() {   
}

} // namespace core
} // namespace swatch
