/* 
 * File:   Utilities.hpp
 * Author: ale
 *
 * Created on July 22, 2014, 5:18 PM
 */

#ifndef __swatch_mp7_utilities_hpp__
#define	__swatch_mp7_utilities_hpp__

// C++ Headers
#include <string>

// Boost Headers
#include <boost/any.hpp>

namespace swatch {
namespace core {
/**
 * 
 * @param iMangledName
 * @return 
 */
std::string demangleName( const char* iMangledName);

/**
 * 
 * @param 
 * @return 
 */
std::string anyToString( const boost::any& );

/**
 * Formats a std::string in printf fashion
 * @param fmt Format string
 * @param ... List of arguments
 * @return A formatted string
 */
std::string strPrintf(const char* aFmt, ...);

/**
 * Sleeps for time in millisecs
 * @param millisec
 */
void millisleep( const double& millisec );

}
}


#endif	/* __swatch_mp7_utilities_hpp__ */

