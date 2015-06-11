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

// BOOST Headers
#include <boost/thread/mutex.hpp>

// Swatch Headers
#include "swatch/core/Functionoid.hpp"
#include "swatch/core/XParameterSet.hpp"

// Forward declarations
namespace xdata {
class Serializable;
}

namespace swatch {
namespace core {

class ActionableObject;

class Command : public Functionoid {
public:

    enum Status {
        kInitial,
        kRunning,
        kError,
        kWarning,
        kDone,
    }; 

    virtual ~Command();

    void exec( XParameterSet& params , const bool& aUseThreadPool = true ); ///Should take const reference but xdata::serializable is const-correctness broken
   
private:
    // thread safe exception catching wrapper for code()
    void runCode( XParameterSet& params );

protected:
    // user defined code for execution
    virtual void code( XParameterSet& params ) = 0; ///Should take const reference but xdata::serializable is const-correctness broken

public:
    virtual void reset();

    Status getStatus() const;

    float getProgress() const;

    xdata::Serializable& getResult();

    template<typename T> T& getResult();

    std::string getProgressMsg();

    std::string getStatusMsg();

    const XParameterSet& getDefaultParams() const;

    template<typename T>
    void registerParameter(const std::string name, const T& defaultValue);

protected:

    /**
     * Merges a parameter set with the default parameter set.
     * Default values are only used if not present in params.
     */
    XParameterSet mergeParametersWithDefaults( XParameterSet& params) const;

    template<typename T>
    Command( const std::string& aId , const T& aDefault );

    xdata::Serializable& defaultResult();

    void setDone( const std::string& aMsg );

    void setWarning( const std::string& aMsg );

    void setError( const std::string& aMsg );

    void setStatus( Status aStatus );
    
    void setProgress( float aProgress );
    
    void setProgress( float aProgress, const std::string& aMsg );

    void setResult( const xdata::Serializable& aResult );

    void setStatusMsg( const std::string& aMsg );

private:
    XParameterSet parameters_;

    Status status_;

    float progress_;

    xdata::Serializable* default_;

    xdata::Serializable* result_;

    std::string progressMsg_;

    std::string statusMsg_;

    // synchronisation
    boost::mutex status_mutex_, progress_mutex_, result_mutex_;

};

typedef boost::unordered_map<std::string, Command*> CommandMap;

} // namespace core
} // namespace swatch
#include "swatch/core/Command.hxx"

#endif /* __SWATCH_CORE_COMMAND_HPP__ */
