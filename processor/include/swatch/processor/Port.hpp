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

    static std::vector<std::string> getDefaultMetrics();

protected:
    
    core::Metric<bool>& metricIsEnabled_;
    core::Metric<bool>& metricIsLocked_;
    core::Metric<bool>& metricIsAligned_;
    core::Metric<uint32_t>& metricCRCErrors_;    
};

class OutputPort : public core::MonitorableObject {
public:
    explicit OutputPort( const std::string& aId );
    explicit OutputPort( const std::string& aId, const core::XParameterSet& params );
    virtual ~OutputPort() {}

    static std::vector<std::string> getDefaultMetrics();

protected:
    core::Metric<bool>& metricIsEnabled_;
    core::Metric<bool>& metricIsOperating_;
};

}
}

#endif	/* __SWATCH_CORE_PORT_HPP__ */

