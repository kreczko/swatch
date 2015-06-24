/**
 * @file    AMC13Commands
 * @details [long description]
 * @author  Alessandro Thea
 * @data    February 2015
 * 
 */

#ifndef __SWATCH_AMC13_AMC13OPERATIONS_HPP__
#define __SWATCH_AMC13_AMC13OPERATIONS_HPP__


#include "swatch/core/Operation.hpp"


namespace swatch {
namespace amc13 {

class AMC13Configure : public swatch::core::Operation {
public:
    AMC13Configure(const std::string& aId);
    virtual ~AMC13Configure();
    
};

} // namespace amc13
} // namespace swatch

#endif /* __SWATCH_AMC13_AMC13OPERATIONS_HPP__ */
