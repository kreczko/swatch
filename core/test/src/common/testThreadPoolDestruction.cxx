

#include "swatch/core/ThreadPool.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"


using namespace swatch::core::test;

struct CommandTestSetup {
  CommandTestSetup():
  handler(),
  print(),
  error(),
  nothing(),
  params(){
    handler.Register<DummyCommand>("dummy_print");//, new DummyCommand(&handler, xdata::Integer(-33)));
    handler.Register<DummyCommand>("dummy_error");//, new DummyCommand(&handler, xdata::Integer(-33)));
    handler.Register<DummyCommand>("dummy_nada");//, new DummyCommand(&handler, xdata::Integer(-33)));

    print = handler.getCommand("dummy_print");
    error = handler.getCommand("dummy_error");
    nothing = handler.getCommand("dummy_nada");

    ((DummyCommand*) print)->registerParameter("todo", xdata::String("print"));
    ((DummyCommand*) error)->registerParameter("todo", xdata::String("error"));
    ((DummyCommand*) nothing)->registerParameter("todo", xdata::String("nothing"));
  }
  ~CommandTestSetup(){
  }

  swatch::core::test::DummyActionableObject handler;
  swatch::core::Command* print, *error, *nothing;
  swatch::core::ReadWriteXParameterSet params;
};


int main( int argc, const char* argv[] )
{
  CommandTestSetup setup;

  swatch::core::ReadWriteXParameterSet params;

  setup.print->exec( setup.params );

  std::cout << "print command being executed " << std::endl;

  //swatch::core::ThreadPool::reset();

  return 0;
}
