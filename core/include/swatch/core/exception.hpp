/**
 * @file    exception.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 *
 */

#ifndef __SWATCH_CORE_EXCEPTION_HPP__
#define __SWATCH_CORE_EXCEPTION_HPP__

// Swatch Headers
// C++ Headers
#include <exception>
#include <string>
#include <sys/time.h>

// Boost Headers
#include <boost/thread.hpp>

//! MACRO to create definitions for the derived exceptions
#define DEFINE_SWATCH_EXCEPTION(T) 				\
	class T : public swatch::core::exception 	\
	{											\
		public:									\
		T() : swatch::core::exception()			\
		{}										\
		T(const std::string& what) :			\
		swatch::core::exception(what)			\
		{}										\
	};


namespace swatch {
namespace core {

class exception : public std::exception {
public:

    /**
     * Constructor
     */
    exception() throw ();

    /**
     * Overloaded constructor
     */
    exception(const std::string& what);

    /**
     * Copy constructor
     * @param e The to copy to build this exception object
     */
    exception(const exception& e) throw ();

    /**
     * Assignment operator
     * @param e The exception object to assign to this one
     * @return Reference to a new exception object
     */
    exception& operator=(const exception& e) throw ();

    /**
     * Destructor. Destruction delegated to subclasses
     */
    virtual ~exception() throw ();

    /**
     * Retrieve the reason for throwing the exception
     * @return const char* containing the reason the exception was thrown
     */
    virtual const char* what() const throw ();


protected:

    //! Thread ID in which the exception was thrown
    boost::thread::id exThreadId_;

    //! Time at which the exception was thrown
    timeval exTime_;

    //! Description of the exception
    std::string what_;


private:

};


//! Add here exception derived classes
// DEFINE_SWATCH_EXCEPTION(ConfigureError);

} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_EXCEPTION_HPP__ */
