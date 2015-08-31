/**
 * @file    CommandSequence.hpp
 * @author  arose
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_COMMANDSEQUENCE_HPP__
#define __SWATCH_CORE_COMMANDSEQUENCE_HPP__


#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/ReadOnlyXParameterSet.hpp"

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/thread/mutex.hpp"

#include <vector>


namespace swatch {
namespace core {

class Command;
class CommandStatus;
class CommandSequenceStatus;
class GateKeeper;
class Operation;


//! Represents a sequence of commands, executed in succession.
class CommandSequence : public Functionoid {
  friend class GateKeeper;
  friend class Operation;

public:
  //! Represents state of command sequence execution
  enum State {
    kInitial,
    kScheduled,
    kRunning,
    kDone,
    kWarning,
    kError
  }; 

  CommandSequence( const std::string& aId, ActionableObject& aResource, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias="");

  CommandSequence( const std::string& aId, ActionableObject& aResource, Command& aFirstCommand, const std::string& aFirstCommandAlias="");

  virtual ~CommandSequence();
  
  /**
    Utility function to add a command to the command sequence
    @param aCommand a command to add to the command sequence
    @param aAlias an alias to use for that command when looking up parameters in gatekeeper; the command's ID string is used if alias is empty
  */
  CommandSequence& run( Command& aCommand, const std::string& aAlias="");
  CommandSequence& then( Command& aCommand, const std::string& aAlias="");
  CommandSequence& operator() ( Command& aCommand, const std::string& aAlias="");

  
  CommandSequence& run( const std::string& aCommand, const std::string& aAlias="");
  CommandSequence& then( const std::string& aCommand, const std::string& aAlias="");
  CommandSequence& operator() ( const std::string& aCommand, const std::string& aAlias="");
  
  const std::vector<std::pair<Command*, std::string> >& getCommands() const;
  
  /**
   * Run the command sequence, extracting the parameters for each command from the supplied gatekeeper
   * 
   * @param aGateKeeper Gatekeeper that will be used to extract the 
   * @param aUseThreadPool Run the sequence asynchronously in the swatch::core::ThreadPool ; if equals false, then the sequence is run synchronously (i.e. only )
   */
  void exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true ); 

  //! Returns current state of this command sequence
  State getState() const;
  
  //! Returns snapshot of this command's current status (state flag value, running time, current command, overall progress fraction)
  CommandSequenceStatus getStatus() const;


private:
//  ///! Reset the commands
//  void reset();

  virtual bool precondition();

  //! thread safe exception-catching wrapper for code()
  void runCommands(boost::shared_ptr<ActionableObject::BusyGuard> aGuard);

  //! Updates the cache of parameter sets used for running commands; throws ParameterNotFound if gatekeeper can't find value of any parameter  
  void updateParameterCache(const GateKeeper& aGateKeeper);

  
  ActionableObject& mResource;  

  typedef std::vector< std::pair< Command*, std::string > > tCommandVector;

  tCommandVector mCommands;

  typedef std::vector< ReadOnlyXParameterSet > tParameterSets;
  tParameterSets mCachedParameters;

  /// The last time a table was updated from the Gatekeeper
  boost::posix_time::ptime mParamUpdateTime;

  mutable boost::mutex mMutex; //mObjectMutex
  
  State mState;

  tCommandVector::iterator mCommandIt;

  boost::posix_time::ptime mExecStartTime;
  boost::posix_time::ptime mExecEndTime;
  
  std::vector<CommandStatus> mStatusOfCompletedCommands;
};

// DEFINE_SWATCH_EXCEPTION( NoParentDefined );

std::ostream& operator<<(std::ostream& out, swatch::core::CommandSequence::State s);

//! Provides a snapshot of the progress/status of a swatch::core::CommandSequence
class CommandSequenceStatus {
public:
  
  const std::string& getSequencePath() const;
    //! Returns state of command sequence execution (scheduled, running, warning, error, done, ...)
    CommandSequence::State getState() const;
    
    //! Returns sequence's running time in seconds
    float getRunningTime() const;
    
    //! Returns fraction progress of sequence - range [0,1] inclusive - 
    float getProgress() const;

    //! Returns count of commands that have already completed execution (regardless of whether their final state is kDone, kWarning, or kError)
    size_t getNumberOfCompletedCommands() const;

    //! Number of commands in the sequence
    size_t getTotalNumberOfCommands() const;
    
    //! Returns results of the command in the sequence that have already completed execution
    const std::vector<const xdata::Serializable*>& getResults() const;

    //! Returns status of commands that have started/completed execution 
    const std::vector<CommandStatus>& getCommandStatus() const;
    
private:
    CommandSequenceStatus(const std::string& aPath, CommandSequence::State aState, float aRunningTime, const Command* aCurrentCommand, const std::vector<CommandStatus>& aFinishedCommandStatuses, size_t aTotalNumberOfCommands);

    std::string mPath;
    CommandSequence::State mState;
    float mRunningTime;
    // const Command* mCurrentCommand;
    size_t mTotalNumberOfCommands;
    std::vector<const xdata::Serializable*> mResults;
    std::vector<CommandStatus> mCommandStatuses;

    friend class CommandSequence;
};

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_COMMANDSEQUENCE_HPP__ */
