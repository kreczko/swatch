
#include <boost/test/unit_test.hpp>

// SWATCH headers
#include "swatch/processor/Port.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"



namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(TestInputPortDefaultMetricsList) {
  std::cout << "ProcessorTestSuite.TestInputPortDefaultMetricsList" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  for(auto it=InputPort::defaultMetrics.begin(); it != InputPort::defaultMetrics.end(); it++)
  {
    BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxA").getMetric(*it));
  }
}


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(TestOutputPortDefaultMetricsList) {
  std::cout << "ProcessorTestSuite.TestOutputPortDefaultMetricsList" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  for(auto it=OutputPort::defaultMetrics.begin(); it != OutputPort::defaultMetrics.end(); it++)
  {
    BOOST_CHECK_NO_THROW(p.getPorts().getOutput("txA").getMetric(*it));
  }
}


BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
