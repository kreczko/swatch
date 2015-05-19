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

#include <vector>
#include <set>


namespace swatch {
namespace core {
  class GateKeeper;


///TODO : In the configure method, who has ownership of the xdata::Serializable* - the source table or the destination parameter set????!
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
    const std::vector<std::string>& getTables();


    Command::Status getStatus() const;

    float getProgress() const;
    float getOverallProgress() const;

    const std::string& getProgressMsg() const;

    const std::string& getStatusMsg() const;

    /**
      Configure the configuration sequence
    */
    virtual void configure();

    /**
      Run the configuration sequence
    */
    virtual void exec(const XParameterSet& params);

    /**
      Reset the configuration commands
    */
    virtual void reset();

  protected:
    virtual std::vector<std::string>* setTables() = 0;


  private:
    void setGateKeeper( GateKeeper* aGateKeeper );

    template< typename T>
    T* cloneSerializable( T* aObj )
    {
      return new T( *aObj );
    } 


    std::vector<std::string>* mTables;
    std::vector< Command* > mCommands;
    std::vector< Command* >::iterator mIt;
    GateKeeper* mGateKeeper;

    static std::string mConfigSequenceComplete;
  };

DEFINE_SWATCH_EXCEPTION( NoGateKeeperDefined );  

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_ConfigSequence_HPP__ */
