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
