/* 
 * File:   Port.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 9:57 AM
 */

#ifndef __swatch_core_port_hpp__
#define	__swatch_core_port_hpp__

#include "swatch/core/Object.hpp"
#include "swatch/core/ParameterSet.hpp"

namespace swatch {
namespace core {

/**
 *  Trigger network port class.
 */
class Port : public Object {
public:
    explicit Port( const std::string& aId, const Arguments& aArguments = Arguments() );
    virtual ~Port();
    
    /// add here methods to do things with a Port
};


class InputPort : public Port {
public:
    explicit InputPort( const std::string& aId, const Arguments& aArguments = Arguments() ) : Port(aId, aArguments) {}
    virtual ~InputPort() {}
};

class OutputPort : public Port {
public:
    explicit OutputPort( const std::string& aId, const Arguments& aArguments = Arguments() ) : Port(aId, aArguments) {}
    virtual ~OutputPort() {}
};


}
}

#endif	/* __swatch_core_port_hpp__ */

