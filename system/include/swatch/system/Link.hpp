/**
 * @file    Link.hpp
 * @author  Alessandro Thea
 * @brief   Connection between Devices in the trigger network
 * @date    20.08.2014
 */

#ifndef __SWATCH_PROCESSOR_LINK_HPP__
#define	__SWATCH_PROCESSOR_LINK_HPP__


#include "swatch/core/Object.hpp"

namespace swatch {
namespace processor {
class Processor;
class InputPort;
class OutputPort;
}

namespace system {

//! Rrepresents the connection between an output and input optical port on different processors

class Link : public core::ObjectView {
public:
  explicit Link(const std::string& aId, processor::Processor* aSrcProcessor, processor::OutputPort* aSrc, processor::Processor* aDstProcessor, processor::InputPort* aDst);

  ~Link();

  processor::Processor* getSrcProcessor();
  processor::OutputPort* getSrcPort();

  processor::Processor* getDstProcessor();
  processor::InputPort* getDstPort();

protected:
  processor::Processor* mSrcProcessor;
  processor::OutputPort* mSrcPort;
  processor::Processor* mDstProcessor;
  processor::InputPort* mDstPort;
};


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_PROCESSOR_LINK_HPP__ */

