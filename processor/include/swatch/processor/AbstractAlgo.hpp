/**
 * @file    AbstractAlgo.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_ABSTRACTALGO_HPP
#define SWATCH_PROCESSOR_ABSTRACTALGO_HPP

// SWATCH Headers
#include "swatch/processor/Component.hpp"

/**
 * @class  AbstractAlgo
 * @author Alessandro Thea 
 * @brief  Abstract class defining the algorithm component interface
 * @date   09/09/2014
 */
class AbstractAlgo : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractAlgo(Connection* connection) : Component(connection) {}

public:
    
    /**
     * Destructor
     */
    virtual ~AbstractAlgo() {}

    void reset() = 0;
};


#endif /* SWATCH_PROCESSOR_ABSTRACTALGO_HPP */