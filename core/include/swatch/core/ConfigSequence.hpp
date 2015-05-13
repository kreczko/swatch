/**
 * @file    ConfigSequence.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_ConfigSequence_HPP__
#define __SWATCH_CORE_ConfigSequence_HPP__

#include "swatch/core/Object.hpp"
#include "swatch/core/Command.hpp"

#include <deque>
#include <set>


//Forward declarations
namespace swatch {
  namespace processor {
    class Processor;
  }
  namespace system {
    class System;
  }
}

namespace swatch {
namespace core {

  class ConfigSequence {
  public:
    /// Constructor
    ConfigSequence( Object& aSystem );
    ConfigSequence( Object& aSystem , Object& aProcessor , const std::string& aHardwareType );

    /// Destructor
    virtual ~ConfigSequence();
  
    /**
      Run the configuration sequence
    */
    virtual void exec();

    /**
      Run the configuration commands
    */
    virtual void reset();


    std::set<std::string> getParams();

    const std::string* getHardwareType();
    const std::string* getSystemId();
    const std::string* getComponentId();


  protected:
    /**
      Utility function to add a command to the command sequence
      @param aCommand a command to add to the command sequence
    */
    void run( Command* aCommand );

  private:
    Object* mProcessor;
    Object* mSystem;

    std::deque< Command* > mCommands;

    std::string* mHardwareType;
    std::string* mSystemId;
    std::string* mComponentId;
  };

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_ConfigSequence_HPP__ */