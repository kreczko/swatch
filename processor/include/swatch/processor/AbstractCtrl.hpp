/**
 * @file    AbstractChannel.hpp
 * @author  Maxime Guilbaud
 * @brief   Brief description
 * @date    24/07/2014
 */

#ifndef SWATCH_PROCESSOR_ABSTRACTCTRL_HPP
#define SWATCH_PROCESSOR_ABSTRACTCTRL_HPP

// OTHER HEADERS
#include "swatch/processor/Component.hpp"

// SWATCH HEADERS
#include "swatch/core/ParameterSet.hpp"

// C++ HEADERS

namespace swatch {
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors
class AbstractCtrl : public Component {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractCtrl(Connection* connection) : Component( connection ) {}

public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractCtrl() {}

    virtual std::set<std::string> clockConfigurations() const = 0;

    virtual void hardReset() = 0;
    virtual void softReset() = 0;

//    virtual void clk40Reset() = 0;
    virtual void configureClock( const std::string& config ) = 0;

};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_ABSTRACTCTRL_HPP */
