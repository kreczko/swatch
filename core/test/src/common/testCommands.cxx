#include <boost/foreach.hpp>
#include <xdata/Integer.h>

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionHandler.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"

namespace swlo = swatch::logger;
namespace swco = swatch::core;

class Other {

};
/**
 * @class DummyResource
 */
class DummyHandler: public Other, public swco::ActionHandler {
public:

  DummyHandler() :
    ActionHandler() {
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

  DummyCommand(swco::ActionHandler* mgr) : Command(mgr, xdata::Integer(-33)) {
    getParameters().adopt("aa", new xdata::Integer(15));
    getParameters().adopt("todo", new xdata::String(""));
  }

  ~DummyCommand() {
  }


  virtual void exec() {

    DummyHandler* res = getHandler<DummyHandler>();

    BOOST_FOREACH(const std::string& n, parameters_.keys()) {
      LOG(swlo::kInfo) << n << " : " << parameters_[n];
    }

    std::string todo = parameters_.get<xdata::String>("todo");
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

  handler.registerCommand("dummy", new DummyCommand(&handler));

  LOG(swlo::kInfo) << "dummy: " << handler.getCommand("dummy");

  swco::Command* cmd = handler.getCommand("dummy");

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->exec();


  LOG(swlo::kInfo) << "progress: " << cmd->progress() << " - " << cmd->progressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->status() << " - " << cmd->statusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->result<xdata::Integer>();

  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";  
  cmd->reset();
  cmd->getParameters().get<xdata::String>("todo") = "print";

  cmd->exec();

  LOG(swlo::kInfo) << "progress: " << cmd->progress() << " - " << cmd->progressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->status() << " - " << cmd->statusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->result<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->reset();
  cmd->getParameters().get<xdata::String>("todo") = "error";
  cmd->exec();

  LOG(swlo::kInfo) << "progress: " << cmd->progress() << " - " << cmd->progressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->status() << " - " << cmd->statusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->result<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  /* code */
  return 0;
}