/* 
 * File:   Link.cpp
 * Author: ale
 */

#include "swatch/system/Link.hpp" 

#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Port.hpp"


namespace swatch {
namespace system {

Link::Link(const std::string& aId, processor::Processor* aSrcProcessor, processor::OutputPort* aSrcPort, processor::Processor* aDstProcessor, processor::InputPort* aDstPort) :
    ObjectView( aId ),
    mSrcProcessor(aSrcProcessor),
    mSrcPort(aSrcPort), 
    mDstProcessor(aDstProcessor),
    mDstPort(aDstPort)
{
    this->addObj(mSrcPort, "src");
    this->addObj(mDstPort, "dst");
}

Link::~Link() {   
}


processor::Processor* Link::getSrcProcessor()
{
  return mSrcProcessor;
}

processor::OutputPort* Link::getSrcPort()
{
  return mSrcPort;
}

processor::Processor* Link::getDstProcessor()
{
  return mDstProcessor;
}

processor::InputPort* Link::getDstPort()
{
  return mDstPort;
}



} // namespace core
} // namespace swatch
