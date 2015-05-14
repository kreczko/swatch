/**
 * @file    Template.hpp
 * @author  Author1, Author2
 * @brief   Brief description
 * @date 
 */

#ifndef SWATCH_PROCESSOR_READOUTINTERFACE_HPP
#define SWATCH_PROCESSOR_READOUTINTERFACE_HPP

#include "swatch/core/Object.hpp"

// Swatch Headers

// C++ Headers


namespace swatch {
namespace processor {

//! @FIXME A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class ReadoutInterface: public core::Object {
protected:

    /**
     * Constructor
     */
    ReadoutInterface() : core::Object( "readout" ) {}

public:

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~ReadoutInterface() {}
};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_READOUTINTERFACE_HPP */
