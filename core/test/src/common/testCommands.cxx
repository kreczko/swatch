#include <boost/foreach.hpp>
#include <xdata/Integer.h>

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionableObject.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"

namespace swlo = swatch::logger;
namespace swco = swatch::core;

/**
 * @class DummyResource
 */
class DummyHandler: public swco::ActionableObject {
public:

  DummyHandler() :
    ActionableObject( "Dummy" ) {
  }
  
  virtual ~DummyHandler() {
  }
  
  
  uint32_t number() const {
    return number_;
  }

  void setNumber(uint32_t number_) {
    this->number_ = number_;
  }

  std::string something() const {
    return something_;
  }

  void setSomething(std::string something_) {
    this->something_ = something_;
  }

private:
  std::string something_;
  uint32_t number_;
};

/**
 * @class DummyCommand
 */
class DummyCommand : public swco::Command {
public:

  DummyCommand( const std::string& aId ) : Command( aId, xdata::Integer(-33)) {
    getParams().adopt("aa", new xdata::Integer(15));
    getParams().adopt("todo", new xdata::String(""));
  }

  ~DummyCommand() {
  }


  virtual void code() {

    DummyHandler* res = getParent<DummyHandler>();

    BOOST_FOREACH(const std::string& n, getParams().keys()) {
      LOG(swlo::kInfo) << n << " : " << getParams()[n];
    }

    std::string todo = getParams().get<xdata::String>("todo");
    if (todo == "print") {

      res->setSomething("|12345|");
      res->setNumber(54);
      setProgress(100.0);
      setResult(xdata::Integer(99));
      setProgress(99., "doing stuff");
      setStatusMsg("Dummy command successfully completed");

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

};



int main(int argc, char const *argv[]) {
  DummyHandler handler;

  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number(); 

  handler.registerCommand<DummyCommand>("dummy");

  LOG(swlo::kNotice) << "Registered commands ";
  BOOST_FOREACH( const std::string& cmdname, handler.getCommands() ) {
    LOG(swlo::kInfo) << " * " << cmdname << ": result type = " << handler.getCommand("dummy")->getResult().type();
  }
  
  LOG(swlo::kInfo) << "dummy: " << handler.getCommand("dummy");

  swco::Command* cmd = handler.getCommand("dummy");

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->exec();


  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();

  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";  
  cmd->reset();
  cmd->getParams().get<xdata::String>("todo") = "print";

  cmd->exec();

  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->reset();
  cmd->getParams().get<xdata::String>("todo") = "error";
  cmd->exec();

  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  /* code */
  return 0;
}
