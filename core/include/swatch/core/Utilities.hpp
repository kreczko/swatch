/**
 * @file    Utilities.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    22/07/14
 */


#ifndef __SWATCH_CORE_UTILITIES_HPP__
#define	__SWATCH_CORE_UTILITIES_HPP__

// C++ Headers
#include <string>
#include <vector>

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

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param aPath [description]
 * @return [description]
 */
std::vector<std::string> shellExpandPaths ( const std::string& aPath );


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param aPath [description]
 * @return [description]
 */
std::string shellExpandPath(const std::string& aPath);

} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_UTILITIES_HPP__ */

