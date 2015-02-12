/* 
 * File:   Port.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 9:57 AM
 */

#ifndef __SWATCH_CORE_PORT_HPP__
#define	__SWATCH_CORE_PORT_HPP__

#include "swatch/core/Object.hpp"

namespace swatch {
namespace core {

/**
 *  Trigger network port class.
 */
class Port : public Object {
public:
    explicit Port( const std::string& aId, const XParameterSet& params = XParameterSet() );
    virtual ~Port();
    
    /// add here methods to do things with a Port
};


class InputPort : public Port {
public:
    explicit InputPort( const std::string& aId, const XParameterSet& params = XParameterSet() ) : Port(aId, params) {}
    virtual ~InputPort() {}
};

class OutputPort : public Port {
public:
    explicit OutputPort( const std::string& aId, const XParameterSet& params = XParameterSet() ) : Port(aId, params) {}
    virtual ~OutputPort() {}
};


}
}

#endif	/* __SWATCH_CORE_PORT_HPP__ */

