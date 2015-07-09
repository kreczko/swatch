/**
 * @file    AlgoInterface.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_ALGOINTERFACE_HPP
#define SWATCH_PROCESSOR_ALGOINTERFACE_HPP


// SWATCH Headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

/**
 * @class  AlgoInterface
 * @author Alessandro Thea 
 * @brief  Abstract class defining the algorithm component interface
 * @date   09/09/2014
 */
class AlgoInterface: public core::MonitorableObject {
public:

    AlgoInterface() : core::MonitorableObject( "algo" ) {}    
    /**
     * Destructor
     */
    virtual ~AlgoInterface() {}

//    virtual void reset() = 0;
};

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_ALGOINTERFACE_HPP */