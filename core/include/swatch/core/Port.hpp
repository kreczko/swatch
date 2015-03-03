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
// class Port : public Object {
// public:
//     explicit Port( const std::string& aId, const XParameterSet& params = XParameterSet() );
//     virtual ~Port();
    
//     /// add here methods to do things with a Port
// };


class InputPort : public Object {
protected:
    explicit InputPort( const std::string& aId) : Object(aId) {}
    explicit InputPort( const std::string& aId, const XParameterSet& params ) : Object(aId, params) {}
public:
    virtual ~InputPort() {}

    virtual bool isEnabled() const = 0;
    virtual bool isOperating() const = 0;
    virtual bool isAligned() const = 0;
    virtual uint32_t getCRCErrors() const = 0;

};

class OutputPort : public Object {
public:
    explicit OutputPort( const std::string& aId ) : Object(aId ) {}
    explicit OutputPort( const std::string& aId, const XParameterSet& params ) : Object(aId, params) {}
    virtual ~OutputPort() {}
};


}
}

#endif	/* __SWATCH_CORE_PORT_HPP__ */

