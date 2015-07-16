

#ifndef __SWATCH_CORE_STANDARDOPERATION_HPP__
#define __SWATCH_CORE_STANDARDOPERATION_HPP__

#include <string>

// Swatch Headers
#include "swatch/core/Operation.hpp"
#include "swatch/core/CommandSequence.hpp"


namespace swatch {
namespace core {

class StandardOperation : public Operation {
public:
  StandardOperation( const std::string& aId );

  virtual ~StandardOperation();

  static const std::string Halted, Synchronised, Connected, Configured, Enabled, Suspended, Failed;
  static const std::string ColdReset, Setup, Connect, Configure, Enable, Suspend, Stop;

private:

  class DummyCommandSequence : public CommandSequence
  {
    public:
      DummyCommandSequence(); 
      void exec();
      std::vector<std::string>* setTables();
  };

  DummyCommandSequence mDummyCommandSequence;
};

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_STANDARDOPERATION_HPP__ */
