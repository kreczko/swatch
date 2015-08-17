/**
 * @file    Command.hpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_COMMAND_HPP__
#define __SWATCH_CORE_COMMAND_HPP__

// C++ Headers
#include <string>
#include <iosfwd>

// BOOST Headers
#include <boost/thread/mutex.hpp>

// Swatch Headers
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/ReadOnlyXParameterSet.hpp"
#include "swatch/core/ReadWriteXParameterSet.hpp"


// Forward declarations
namespace xdata {
class Serializable;
}


namespace swatch {
namespace core {


class ActionableObject;
class CommandStatus;
class CommandSequence;


//! Represents a one-shot action on a resource (a class that inherits from swatch::core::ActionableObject)
class Command : public Functionoid {
public:

    enum State {
        kInitial,
        kScheduled,
        kRunning,
        kError,
        kWarning,
        kDone,
    }; 

    virtual ~Command();

    /** 
     * Run this command, using the supplied set of parameters
     * 
     * @param aParams Map of parameter values; any default parameters for this command that aren't included in this argument will be merged into the set supplied to the code method
     * @param aUseThreadPool Run the command asynchronously in the swatch::core::ThreadPool ; if equals false, then the command is run synchronously
     */
    void exec( const XParameterSet& aParams , const bool& aUseThreadPool = true );

    //! Returns current state of this command
    State getState() const;

    //! Returns snapshot of this command's current status (state flag value, running time, progress fraction, status message and result) as a CommandStatus instance
    CommandStatus getStatus() const;
    
    template<typename T>
    void registerParameter(const std::string name, const T& defaultValue);

    const ReadWriteXParameterSet& getDefaultParams() const;
    
    const xdata::Serializable& getDefaultResult() const;

protected:
    //! user-defined code for execution
    virtual State code( const XParameterSet& params ) = 0;

    template<typename T>
    Command( const std::string& aId , const T& aDefault );

    //! Set command's current fractional progress; valid range [0,1]
    void setProgress( float aProgress );
    
    //! Set command's current fractional progress (valid range [0,1]), and status message
    void setProgress( float aProgress, const std::string& aMsg );

    //! Set value of result from current command execution
    void setResult( const xdata::Serializable& aResult );

    //! Set command's status message
    void setStatusMsg( const std::string& aMsg );

private:

    void reset();

    /**
     * Merges a parameter set with the default parameter set.
     * Default values are only used if not present in params.
     */
    ReadOnlyXParameterSet mergeParametersWithDefaults(const XParameterSet& params) const;

    // thread safe exception catching wrapper for code()
    void runCode( const XParameterSet& params );

    ReadWriteXParameterSet defaultParams_;

    ReadOnlyXParameterSet runningParams_;

    xdata::Serializable* const defaultResult_;

    State state_;

    timeval execStartTime_;
    timeval execEndTime_;

    float progress_;

    std::string statusMsg_;

    boost::shared_ptr<xdata::Serializable> result_;

    // Mutex for thread safety of read-/write-access of member data read-/write-access
    mutable boost::mutex mutex_;
    
    //! Creates copy of xdata::Serializable object of type T on heap
    template<typename T>
    static xdata::Serializable* cloneSerializable_( const xdata::Serializable* other );

    typedef xdata::Serializable* (*ResultXCloner)( const xdata::Serializable* );

    //! Used to clone default result into result_ at start of execution, just before the code method is called
    ResultXCloner resultCloner_;
    
    friend class CommandSequence;
};

std::ostream& operator<<(std::ostream& out, swatch::core::Command::State s);


//! Provides a snapshot of the progress/status of a swatch::core::Command
class CommandStatus {
    
public:
    //! Returns state of command execution (scheduled, running, warning, error, done, ...)
    Command::State getState() const;
    
    //! Returns command's running time in seconds
    float getRunningTime() const;

    //! Returns fractional progress of command; range [0,1]
    float getProgress() const;
    
    //! Returns command's status message
    const std::string& getStatusMsg() const;
    
    //! Returns command's result; equal to NULL if command has not already completed execution
    const xdata::Serializable* const getResult() const;
    
    std::string getResultAsString() const;
    
private:
    CommandStatus(Command::State aState, float aRunningTime, float aProgress, const std::string& aStatusMsg, const boost::shared_ptr<xdata::Serializable>& aResult);

    Command::State state_;
    float runningTime_;
    float progress_;
    std::string statusMsg_;
    boost::shared_ptr<xdata::Serializable> result_;

    friend class Command;
};

} // namespace core
} // namespace swatch


#include "swatch/core/Command.hxx"


#endif /* __SWATCH_CORE_COMMAND_HPP__ */
