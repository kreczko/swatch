/* 
 * File:   Port.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 9:57 AM
 */

#ifndef __SWATCH_CORE_PORT_HPP__
#define	__SWATCH_CORE_PORT_HPP__

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace processor {

class InputPort : public core::MonitorableObject {
protected:
    explicit InputPort( const std::string& aId);
    explicit InputPort( const std::string& aId, const core::XParameterSet& params );
public:
    virtual ~InputPort() {}

    virtual bool isEnabled() const = 0;
    virtual bool isLocked() const = 0;
    virtual bool isAligned() const = 0;
    virtual uint32_t getCRCErrors() const = 0;
    
    static std::vector<std::string> getDefaultMetrics();
};

class OutputPort : public core::MonitorableObject {
public:
    explicit OutputPort( const std::string& aId );
    explicit OutputPort( const std::string& aId, const core::XParameterSet& params );
    virtual ~OutputPort() {}

    virtual bool isEnabled() const = 0;
    virtual bool isOperating() const = 0;
    
    static std::vector<std::string> getDefaultMetrics();
};

}
}

#endif	/* __SWATCH_CORE_PORT_HPP__ */

