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
#include "swatch/core/exception.hpp"

#include <deque>
#include <set>


namespace swatch {
namespace core {
  class GateKeeper;


  class ConfigSequence : public Functionoid {
  friend class GateKeeper;  

  public:
    /// Constructor
    ConfigSequence( const std::string& aId );

    /// Destructor
    virtual ~ConfigSequence();
  
    /**
      Utility function to add a command to the command sequence
      @param aCommand a command to add to the command sequence
    */
    ConfigSequence& run( Command* aCommand );
    ConfigSequence& then( Command* aCommand );
    ConfigSequence& operator() ( Command* aCommand );

    ConfigSequence& run( const std::string& aCommand );
    ConfigSequence& then( const std::string& aCommand );
    ConfigSequence& operator() ( const std::string& aCommand );

    std::set<std::string> getParams();
    const std::deque<std::string>& getTables();

  protected:
    virtual void setTables() = 0;
    std::deque<std::string>* mTables;

  private:
    /**
      Run the configuration sequence
    */
    virtual void exec();

    /**
      Run the configuration commands
    */
    virtual void reset();

  private:
    std::deque< Command* > mCommands;
  };

DEFINE_SWATCH_EXCEPTION( UnknownParentType );  

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_ConfigSequence_HPP__ */
