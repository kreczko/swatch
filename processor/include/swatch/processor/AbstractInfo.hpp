/* 
 * File:   AbstractInfo.hpp
 * Author: mguilbau
 *
 * Created on 7 août 2014, 11:35
 */

#ifndef ABSTRACTINFO_HPP
#define	ABSTRACTINFO_HPP

// OTHER HEADERS
#include "swatch/processor/Connection.hpp"

// SWATCH HEADERS
#include "swatch/core/ParameterSet.hpp"

// C++ HEADERS

namespace swatch {

// CTRLNODE CLASS
namespace processor {

class AbstractInfo {
protected:
    /**
     * Constructor
     * @param connexion object to connect to the hardware
     */
    AbstractInfo(Connection* connection);

    Connection* connection() {
        return connection_;
    }
    
    uint32_t magicNumber() {
        return magicNumber_;
    }
    
    uint32_t fwVersion() {
        return fwversion_;
    }
    
public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
    */
    virtual ~AbstractInfo();
    
    virtual uint32_t getMagicNumber() = 0;
    virtual uint32_t getFwVersion() = 0;

protected:
    uint32_t magicNumber_;
    uint32_t fwversion_;
private:
    Connection* connection_;


};

} // end ns processor
} // end ns swatch
#endif	/* ABSTRACTINFO_HPP */

