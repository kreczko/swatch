/**
 * @file    Command.hpp
 * @author  Alessandro Thea
 * @brief   Command, class representing the trigger network node.
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_TEST_COMMAND__
#define __SWATCH_CORE_TEST_COMMAND__

// C++ Headers
#include <string>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"

// Forward declarations
namespace xdata {
class Serializable;
}

namespace swatch {
namespace core {

class ActionHandler;

class Command {
public:

    enum Status {
        kInitial,
        kRunning,
        kError,
        kWarning,
        kDone,
    }; 

    virtual ~Command();

    void exec();
   
protected:
    virtual void code() = 0;

public:
    virtual void reset();

    XParameterSet & getParams();

    Status status() const;

    float progress() const;

    xdata::Serializable& result();

    template<typename T>
    T& result() {
        // T must be derived from xdata::Serializable 
        BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
        return dynamic_cast<T&>(result());
    }

    const std::string& progressMsg() const;

    const std::string& statusMsg() const { return statusMsg_; }

protected:

    template<typename T>
    Command( ActionHandler* aHandler, const T& aDefault ) :
        default_(new T(aDefault)), 
        result_(new T()),
        handler_(aHandler) {
        // T must be derived from xdata::Serializable 
        BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

        reset();
    }
    
    template<typename H>
    H* getHandler() {
        return dynamic_cast<H*>(handler_);
    }

    xdata::Serializable& defaultResult() { return *default_; }

    void setDone( const std::string& aMsg );

    void setWarning( const std::string& aMsg );

    void setError( const std::string& aMsg );

    void setStatus( Status aStatus ) { status_ = aStatus; }
    
    void setProgress( float aProgress );
    
    void setProgress( float aProgress, const std::string& aMsg );

    void setResult( const xdata::Serializable& aResult ) {
        result_->setValue(aResult);
    }

    void setStatusMsg( const std::string& aMsg );

    XParameterSet parameters_;

private:

    Status status_;

    float progress_;

    xdata::Serializable* default_;

    xdata::Serializable* result_;

    std::string progressMsg_;

    std::string statusMsg_;

    ActionHandler* handler_;

};            

} // namespace core
} // namespace swatch

#endif
