/*
 * DummyProcessorOperation.hpp
 *
 *  Created on: 3 Mar 2015
 *      Author: kreczko
 */

#ifndef SWATCH_PROCESSOR_TEST_DUMMYPROCESSOROPERATION_HPP
#define SWATCH_PROCESSOR_TEST_DUMMYPROCESSOROPERATION_HPP
#include "swatch/core/Operation.hpp"

namespace swatch {
namespace processor {
namespace test {
class DummyProcessorOperation: public swatch::core::Operation {
public:
  DummyProcessorOperation(swatch::core::ActionableObject* resource);
  virtual ~DummyProcessorOperation();

  // current set of 'operations' we want the operation to execute
  virtual void test(toolbox::Event::Reference event);

};
}
}
}

#endif /* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOROPERATION_HPP */
