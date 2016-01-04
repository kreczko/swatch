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

//! Rrepresents the connection between an output and input optical port on different processors
class Link : public core::ObjectView {
public:
    explicit Link( const std::string& aId, OutputPort* aSrc, InputPort* aDst );

    ~Link();
    
    OutputPort* getSrc() { return mSrc; }

    InputPort* getDst() { return mDst; }
    
protected:
    OutputPort* mSrc;
    InputPort* mDst;
};


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_PROCESSOR_LINK_HPP__ */

