/**
 * @file    AlgoInterface.cpp
 * @author  Alessandro Thea
 */

#ifndef SWATCH_PROCESSOR_ALGOINTERFACE_HPP
#define SWATCH_PROCESSOR_ALGOINTERFACE_HPP


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

/**
 * @class  AlgoInterface
 * @author Alessandro Thea 
 * @brief  Abstract class defining the algorithm component interface
 */
class AlgoInterface: public core::MonitorableObject {
public:

    AlgoInterface() : core::MonitorableObject( "algo" ) {}    

    virtual ~AlgoInterface() {}

//    virtual void reset() = 0;
};

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_ALGOINTERFACE_HPP */