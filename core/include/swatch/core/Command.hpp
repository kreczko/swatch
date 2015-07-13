/**
 * @file    Command.hpp
 * @author  Alessandro Thea
 * @brief   Command, class representing the trigger network node.
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
#include "swatch/core/XParameterSet.hpp"
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

    void exec( const XParameterSet& params , const bool& aUseThreadPool = true );
   
    virtual void reset();

    State getState() const;

    CommandStatus getStatus() const;
    
    template<typename T>
    void registerParameter(const std::string name, const T& defaultValue);

    const ReadWriteXParameterSet& getDefaultParams() const;
    
    const xdata::Serializable& getDefaultResult() const;

protected:
    // user defined code for execution
    virtual State code( const XParameterSet& params ) = 0;

    template<typename T>
    Command( const std::string& aId , const T& aDefault );

    xdata::Serializable& defaultResult();

    void setProgress( float aProgress );
    
    void setProgress( float aProgress, const std::string& aMsg );

    void setResult( const xdata::Serializable& aResult );

    void setStatusMsg( const std::string& aMsg );

private:
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
};

std::ostream& operator<<(std::ostream& out, swatch::core::Command::State s);

typedef boost::unordered_map<std::string, Command*> CommandMap;


//! Provides a snapshot of the progress/status of a swatch::core::Command
class CommandStatus {
    
public:
    //! Returns status of command execution
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

    const Command::State state_;
    const float runningTime_;
    const float progress_;
    const std::string statusMsg_;
    boost::shared_ptr<xdata::Serializable> result_;

    friend class Command;
};

} // namespace core
} // namespace swatch


#include "swatch/core/Command.hxx"


#endif /* __SWATCH_CORE_COMMAND_HPP__ */
