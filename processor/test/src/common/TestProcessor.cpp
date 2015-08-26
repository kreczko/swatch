
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/test/DummyAlgo.hpp"
#include "swatch/processor/test/DummyReadout.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyTTC.hpp"



namespace swatch {
namespace processor {
namespace test {

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(TestProcessorComponentAccess) {
  std::cout << "ProcessorTestSuite.TestProcessorComponentAccess" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  BOOST_CHECK( typeid(p.getAlgo()) == typeid(DummyAlgo) );
  BOOST_CHECK( typeid(p.getReadout()) == typeid(DummyReadoutInterface) );
  BOOST_CHECK( typeid(p.getTTC()) == typeid(DummyTTC) );
  BOOST_CHECK( typeid(p.getPorts()) == typeid(swatch::processor::PortCollection) );
}


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(CreationTest) {
  std::cout << "ProcessorTestSuite.CreationTest" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  BOOST_CHECK_EQUAL(p.getSlot(), uint32_t(0));
  BOOST_CHECK_EQUAL(p.getSlot(), p.getStub().slot);
  BOOST_CHECK_EQUAL(p.getCrateId(), "s2g20-10");
  BOOST_CHECK_EQUAL(p.getCrateId(), p.getStub().crate);

  // Input and output ports
  BOOST_CHECK_EQUAL(p.getPorts().getNumInputs(), size_t(5));
  BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxA"));
  BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxB"));
  BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxC"));
  BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxD"));
  BOOST_CHECK_NO_THROW(p.getPorts().getInput("rxE"));

  BOOST_CHECK_EQUAL(p.getPorts().getNumOutputs(), size_t(3));
  BOOST_CHECK_NO_THROW(p.getPorts().getOutput("txA"));
  BOOST_CHECK_NO_THROW(p.getPorts().getOutput("txB"));
  BOOST_CHECK_NO_THROW(p.getPorts().getOutput("txC"));
}


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(TestDefaultMetricsList) {
  std::cout << "ProcessorTestSuite.TestDefaultMetricsList" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  for(auto it=Processor::defaultMetrics.begin(); it != Processor::defaultMetrics.end(); it++)
  {
    BOOST_CHECK_NO_THROW(p.getMetric(*it));
  }
}
      

//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(TestDefaultMonitorableObjectsList) {
  std::cout << "ProcessorTestSuite.TestDefaultMonitorableObjectsList" << std::endl;

  DummyProcessor p(DummyProcessor::generateParams("dummy"));

  for(auto it=Processor::defaultMonitorableObjects.begin(); it != Processor::defaultMonitorableObjects.end(); it++)
  {
    BOOST_CHECK_NE(p.getObj<core::MonitorableObject>(*it), (swatch::core::MonitorableObject*)NULL);
  }
}


BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
