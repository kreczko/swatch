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
class InputPort;
class OutputPort;
}

namespace system {

//! Rrepresents the connection between an output and input optical port on different processors
class Link : public core::ObjectView {
public:
    explicit Link( const std::string& aId, processor::OutputPort* aSrc, processor::InputPort* aDst );

    ~Link();
    
    processor::OutputPort* getSrc() { return mSrc; }

    processor::InputPort* getDst() { return mDst; }
    
protected:
    processor::OutputPort* mSrc;
    processor::InputPort* mDst;
};


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_PROCESSOR_LINK_HPP__ */

