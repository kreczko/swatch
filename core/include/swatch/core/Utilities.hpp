/**
   @file    Utilities.hpp
   @author  Alessandro Thea
   @brief   Brief description
   @date    22/07/14
 */


#ifndef __SWATCH_CORE_UTILITIES_HPP__
#define	__SWATCH_CORE_UTILITIES_HPP__

// C++ Headers
#include <string>
#include <vector>

// Boost Headers
#include <boost/any.hpp>

namespace swatch
{
  namespace core
  {
    /**
      Back-convert GCC-mangled names to class names
      @param aMangledName a GCC-mangled name
      @return the original class names
     */
    std::string demangleName ( const char* aMangledName );

    /**
      Convert a boost::any to a string representation
      @param aAny a boost::any to convert to a string
      @return a string representation of the boost::any
     */
    std::string anyToString ( const boost::any& aAny );

    /**
      Formats a std::string in printf fashion
      @param aFmt Format string
      @param ... List of arguments
      @return A formatted string
    */
    std::string strPrintf ( const char* aFmt, ... );

    /**
      Sleeps for time in millisecs
      @param aMillisec a time in millisecs for which to sleep
    */
    void millisleep ( const double& aMillisec );

    /**
      Expand a shell expression (stars, environment variables, etc) to a list of file paths
      @param aPath a shell expression to expand
      @return a list of all matching file paths, throw if no results found
    */
    std::vector<std::string> shellExpandPaths ( const std::string& aPath );


    /**
      Expand a shell expression (stars, environment variables, etc) to a file path
      @param aPath a shell expression to expand
      @return a file path, throw if no results or more than one result found
    */
    std::string shellExpandPath ( const std::string& aPath );

  } // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_UTILITIES_HPP__ */

