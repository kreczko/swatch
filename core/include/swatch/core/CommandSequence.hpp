/**
 * @file    CommandSequence.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_CommandSequence_HPP__
#define __SWATCH_CORE_CommandSequence_HPP__

#include "swatch/core/Object.hpp"
#include "swatch/core/Command.hpp"

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/thread/mutex.hpp>
// #include "boost/any.hpp"


#include <vector>
#include <set>


namespace swatch {
namespace core {
  class GateKeeper;
  class Operation;


///TODO : In the configure method, who has ownership of the xdata::Serializable* - the source table or the destination parameter set????!
  class CommandSequence : public Functionoid {
    friend class GateKeeper;
    friend class Operation;

  public:

    enum Status {
        kDone,
        kRunning,
        kFailed
    }; 

    /// Constructor
    CommandSequence( const std::string& aId );

    /// Destructor
    virtual ~CommandSequence();
  
    /**
      Utility function to add a command to the command sequence
      @param aCommand a command to add to the command sequence
    */
    CommandSequence& run( Command* aCommand );
    CommandSequence& then( Command* aCommand );
    CommandSequence& operator() ( Command* aCommand );

    CommandSequence& run( const std::string& aCommand );
    CommandSequence& then( const std::string& aCommand );
    CommandSequence& operator() ( const std::string& aCommand );

//     std::set<std::string> getParams();
    const std::vector<std::string>& getTables();


    const Command* getCurrentCommand();

    Status getStatus() const;

    float getProgress();
    std::vector< const xdata::Serializable* > getResults();

    /**
      Run the configuration sequence
    */
    virtual void exec( const bool& aUseThreadPool = true ); 

    /**
      Reset the configuration commands
    */
    virtual void reset();

  protected:
    virtual std::vector<std::string>* setTables() = 0;


  private:
    virtual bool precondition();
    virtual void threadless_exec(); 

    // thread safe exception catching wrapper for code()
    void runCode();

    void setGateKeeper( GateKeeper* aGateKeeper );

    void UpdateParameterCache();


//     Command& getCommand( boost::any& aCommand ) const;
    Command& getCommand( Command* aCommand ) const;


    std::vector<std::string>* mTables;

    typedef std::vector< Command* > tCommandVector;
//     typedef std::vector< boost::any > tCommandVector;

    tCommandVector mCommands;
    tCommandVector::iterator mIt;

    GateKeeper* mGateKeeper;

    typedef std::vector< XParameterSet > tParameterSets;
    tParameterSets mCachedParameters;

//     static std::string mCommandSequenceComplete;

    /// The last time a table was updated from the Gatekeeper
    boost::posix_time::ptime mUpdateTime;

    boost::mutex mIteratorMutex ; //mObjectMutex

    Status mStatus;
  };

DEFINE_SWATCH_EXCEPTION( NoGateKeeperDefined );  
DEFINE_SWATCH_EXCEPTION( NoParentDefined );  

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_CommandSequence_HPP__ */
