/**
 * @file    Link.hpp
 * @author  Alessandro Thea
 * @brief   Connection between Devices in the trigger network
 * @date    20.08.2014
 *
 * Detailed description
 */

#ifndef __SWATCH_CORE_LINK_HPP__
#define	__SWATCH_CORE_LINK_HPP__


#include "swatch/core/Device.hpp"

namespace swatch {
namespace core {

class InputPort;
class OutputPort;

class Link : public ObjectView {
public:
    explicit Link( const std::string& aId, OutputPort* aSrc, InputPort* aDst, const ParameterSet& params  = ParameterSet() );
    ~Link();
    
    OutputPort* getSrc() { return src_; }
    InputPort* getDst() { return dst_; }
    
protected:
    OutputPort* src_;
    InputPort* dst_;
};


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_LINK_HPP__ */

