/* 
 * File:   Port.hpp
 * Author: ale
 */

#ifndef __SWATCH_PROCESSOR_PORT_HPP__
#define	__SWATCH_PROCESSOR_PORT_HPP__


#include "swatch/core/MaskableObject.hpp"
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

//! Base class defining the interface of an input optical port 
class InputPort : public core::MaskableObject {
protected:
    explicit InputPort( const std::string& aId);

public:
    virtual ~InputPort() {}

    static const std::vector<std::string> kDefaultMetrics;

protected:
    core::Metric<bool>& mMetricIsLocked;
    core::Metric<bool>& mMetricIsAligned;
    core::Metric<uint32_t>& mMetricCRCErrors;    
};


//! Base class defining the interface of an output optical port 
class OutputPort : public core::MonitorableObject {
protected:
    explicit OutputPort( const std::string& aId );

public:
    virtual ~OutputPort() {}

    static const std::vector<std::string> kDefaultMetrics;

protected:
    core::Metric<bool>& mMetricIsOperating;
};

}
}

#endif	/* __SWATCH_PROCESSOR_PORT_HPP__ */

