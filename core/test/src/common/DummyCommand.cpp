#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>

using namespace swatch;
namespace swatch {
namespace core {
namespace test {

DummyCommand::DummyCommand(  const std::string& aId ) :
        core::Command( aId , xdata::Integer(-33)) {
  getParams().adopt("aa", new xdata::Integer(15));
  getParams().adopt("todo", new xdata::String(""));
}

DummyCommand::~DummyCommand() {
//  delete dummy_proc_;
}

void DummyCommand::code() {
  DummyHandler* res = getParent<DummyHandler>();

  BOOST_FOREACH(const std::string& n, parameters_.keys()) {
    LOG(logger::kInfo) << n << " : " << parameters_[n];
  }

  std::string todo = parameters_.get<xdata::String>("todo");
  if (todo == "print") {

    res->setSomething("|12345|");
    res->setNumber(54);
    setProgress(100.0);
    setResult(xdata::Integer(99));
    setProgress(99., "doing stuff");
    setDone("Dummy command successfully completed");

  } else if (todo == "error") {
    //      setProgress(50.49);
    setProgress(50.49, "Dummy command did something");
    setError("But ended up in error");

  } else {
    //      setProgress(0.0);
    setProgress(0.0, "Not even started");
    setWarning("Nothing was done");

  }
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
