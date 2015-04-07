/**
 * @file    AMC13Commands
 * @details [long description]
 * @author  Alessandro Thea
 * @data    February 2015
 * 
 */

#ifndef __SWATCH_HARDWARE_AMC13OPERATIONS_HPP__
#define __SWATCH_HARDWARE_AMC13OPERATIONS_HPP__

#include "swatch/core/Operation.hpp"

namespace swatch {
namespace hardware {

class AMC13Configure : public swatch::core::Operation {
public:
    AMC13Configure(core::ActionHandler* aHandler);
    virtual ~AMC13Configure();
    
};

} // namespace hardware
} // namespace swatch

#endif /* __SWATCH_HARDWARE_AMC13OPERATIONS_HPP__ */