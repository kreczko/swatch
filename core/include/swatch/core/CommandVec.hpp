/**
 * @file    CommandSequence.hpp
 * @author  arose
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_COMMANDVEC_HPP__
#define __SWATCH_CORE_COMMANDVEC_HPP__


#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/ReadOnlyXParameterSet.hpp"

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/thread/mutex.hpp"

#include <vector>


namespace swatch {
namespace core {

class ActionableSystem;
class Command;
class CommandStatus;
class CommandVecStatus;
class GateKeeper;

//! Represents a sequence of commands, executed in succession.
class CommandVec : public ObjectFunctionoid {
protected:
  CommandVec( const std::string& aId, ActionableObject& aResource);
  
  void addCommand(Command& aCommand, const std::string& aNamespace);
  
  void addCommand(const std::string& aCommand, const std::string& aNamespace);

public:  
  virtual ~CommandVec();
  
  class Element {
  public:
    Element(Command& aCommand, const std::string& aNamespace);
    ~Element();
    
    const std::string& getNamespace() const;
    
    const Command& get() const;
    
  private:
    Command* mCmd;
    std::string mNamespace;
    friend class CommandVec;
  };

//  const ActionableObject& getActionable() const;
  
//  ActionableObject& getActionable();
  
  //! Number of commands
  size_t size() const;

  typedef std::vector<Element>::const_iterator const_iterator;

  const_iterator begin() const;
  const_iterator end() const;

  std::vector<Command*> getCommands();
  
  /**
   * Run the sequence, extracting the parameters for each command from the supplied gatekeeper
   * 
   * @param aGateKeeper Gatekeeper that's used to extract the parameters
   * @param aUseThreadPool Run the sequence asynchronously in the swatch::core::ThreadPool ; if equals false, then the sequence is run synchronously (i.e. only )
   */
  void exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true ); 

  /** 
   * Run this sequence from another functionoid that already has control of resource, extracting the parameters for each command from the supplied gatekeeper
   * 
   * @param aGateKeeper Gatekeeper that's used to extract the parameters
   * @param aUseThreadPool Run the command asynchronously in the swatch::core::ThreadPool ; if equals false, then the command is run synchronously
   */
  void exec(const ActionableObject::BusyGuard* aGuard, const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true );

  //! Returns current state of this command sequence
  ActionStatus::State getState() const;
  
  //! Returns snapshot of this command's current status (state flag value, running time, current command, overall progress fraction)
  CommandVecStatus getStatus() const;

  struct MissingParam 
  {
    MissingParam(const std::string& aNamespace, const std::string& aCommand, const std::string& aParam);
    std::string nspace;
    std::string command;
    std::string parameter;
  };

  void checkForMissingParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<MissingParam>& aMissingParams) const;

private:
  /*!
   * Extracts from gatekeeper the parameter sets for running commands 
   * @param aParamSets vector containing the extracted parameter sets
   * @param aMissingParams vector of missing parameters for each command
   * @param aThrowOnMissing if true, then throws ParameterNotFound if gatekeeper can't find value of any parameter
   */
  void extractParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<MissingParam>& aMissingParams, bool throwOnMissing) const;

  //! thread safe exception-catching wrapper for code()
  void runCommands(boost::shared_ptr<ActionableObject::BusyGuard> aGuard);

//  ActionableObject& mResource;

  MutableActionableStatus& mActionableStatus;
  typedef std::vector< Element > tCommandVector;
  tCommandVector mCommands;

  typedef tReadOnlyXParameterSets tParameterSets;
  tParameterSets mCachedParameters;

  tMonitoringSettings mCachedMonitoringSettings;

  /// The last time a table was updated from the Gatekeeper
  boost::posix_time::ptime mParamUpdateTime;

  mutable boost::mutex mMutex; //mObjectMutex
  
  ActionStatus::State mState;

  tCommandVector::iterator mCommandIt;

  boost::posix_time::ptime mExecStartTime;
  boost::posix_time::ptime mExecEndTime;
  
  std::vector<CommandStatus> mStatusOfCompletedCommands;
  
//  GateKeeper mCachedGateKeeper;

protected:
  virtual void prepareCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) = 0;
  virtual void finaliseCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) = 0;
  virtual void extractMonitoringSettings(const GateKeeper& aGateKeeper, tMonitoringSettings& aMonSettings) const = 0;

};


std::ostream& operator << (std::ostream& aOstream, const CommandVec::MissingParam& aMissingParam );

bool operator !=(const CommandVec::MissingParam& l1, const CommandVec::MissingParam& l2);


//! Provides a snapshot of the progress/status of a swatch::core::CommandVec
class CommandVecStatus : public ActionStatus {
public:
  
    //! Returns fraction progress of sequence - range [0,1] inclusive
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
    CommandVecStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, const Command* aCurrentCommand, const std::vector<CommandStatus>& aFinishedCommandStatuses, size_t aTotalNumberOfCommands);

    // const Command* mCurrentCommand;
    size_t mTotalNumberOfCommands;
    std::vector<const xdata::Serializable*> mResults;
    std::vector<CommandStatus> mCommandStatuses;

    friend class CommandVec;
};

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_COMMANDVEC_HPP__ */
