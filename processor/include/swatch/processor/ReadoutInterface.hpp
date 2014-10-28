/**
 * @file    Template.hpp
 * @author  Author1, Author2
 * @brief   Brief description
 * @date 
 */

#ifndef SWATCH_PROCESSOR_READOUTINTERFACE_HPP
#define SWATCH_PROCESSOR_READOUTINTERFACE_HPP


// Swatch Headers
#include "swatch/processor/Component.hpp"

// C++ Headers
namespace swatch {
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class ReadoutInterface : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    ReadoutInterface(Connection* connection) : Component(connection) {}

public:

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    ~ReadoutInterface() {}
};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_READOUTINTERFACE_HPP */
