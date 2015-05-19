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

    void exec( XParameterSet& params); ///Should take const reference but xdata::serializable is const-correctness broken
   
protected:
    virtual void code( XParameterSet& params ) = 0; ///Should take const reference but xdata::serializable is const-correctness broken

public:
    virtual void reset();

    Status getStatus() const;

    float getProgress() const;

    xdata::Serializable& getResult();

    template<typename T> T& getResult();

    const std::string& getProgressMsg() const;

    const std::string& getStatusMsg() const;

protected:

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

    Status status_;

    float progress_;

    xdata::Serializable* default_;

    xdata::Serializable* result_;

    std::string progressMsg_;

    std::string statusMsg_;

};

typedef boost::unordered_map<std::string, Command*> CommandMap;

} // namespace core
} // namespace swatch
#include "swatch/core/Command.hxx"

#endif /* __SWATCH_CORE_COMMAND_HPP__ */
