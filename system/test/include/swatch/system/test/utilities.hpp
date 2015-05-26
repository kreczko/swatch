/**
 * @file    utilities.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef SWATCH_PROCESSOR_TEST_UTILITIES_HPP
#define SWATCH_PROCESSOR_TEST_UTILITIES_HPP

// Swatch Headers
#include "swatch/processor/ProcessorStub.hpp"

namespace swatch {
namespace system {
namespace test {

    //! Add a dummy set of input and output port stubs to a processor stub, for system construction unit tests
    void addRxTxPortStubs(swatch::processor::ProcessorStub& aProcStub);

} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_UTILITIES_HPP */

