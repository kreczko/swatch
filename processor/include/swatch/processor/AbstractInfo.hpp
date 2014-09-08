/* 
 * File:   AbstractInfo.hpp
 * Author: mguilbau
 *
 * Created on 7 août 2014, 11:35
 */

#ifndef SWATCH_PROCESSOR_ABSTRACTINFO_HPP
#define	SWATCH_PROCESSOR_ABSTRACTINFO_HPP


// Swatch Headers
#include "swatch/processor/Component.hpp"
#include "swatch/core/ParameterSet.hpp"

// C++ Headers

namespace swatch {
namespace processor {

class AbstractInfo : public Component {
protected:
    /**
     * Constructor
     * @param connexion object to connect to the hardware
     */
    AbstractInfo(Connection* connection) : Component(connection) {}
    
public:

    virtual ~AbstractInfo() {}
    
    /**
     * 
     * @return Firmware version
     */
    virtual uint32_t getFwVersion() = 0;

    virtual uint32_t getNInputs() = 0;
    
    virtual uint32_t getNOutputs() = 0;
    
};

} // end ns processor
} // end ns swatch
#endif	/* SWATCH_PROCESSOR_ABSTRACTINFO_HPP */

