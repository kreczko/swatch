#include "swatch/core/Utilities.hpp"

// C++
#include <cxxabi.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <cstdarg>
#include <wordexp.h>
#include <stdexcept>

namespace swatch
{
  namespace core
  {

    std::string demangleName ( const char* aMangledName )
    {
      int status = 0;
      char* demangled = abi::__cxa_demangle ( aMangledName, 0, 0, &status );
      const std::string name ( status == 0 ? demangled : "[UNKNOWN]" );

      if ( ( status == 0 ) and ( NULL != demangled ) )
      {
        free ( demangled );
      }

      return name;
    }

    std::string strPrintf ( const char* aFmt, ... )
    {
      char* ret;
      va_list ap;
      va_start ( ap, aFmt );
      vasprintf ( &ret, aFmt, ap );
      va_end ( ap );
      std::string str ( ret );
      free ( ret );
      return str;
    }

    void millisleep ( const double& aMillisec )
    {
      //  using namespace uhal;
      //  logging();
      double secs ( aMillisec / 1e3 );
      int intPart ( ( int ) secs );
      double fracPart ( secs - ( double ) intPart );
      struct timespec sleepTime, returnTime;
      sleepTime.tv_sec = intPart;
      sleepTime.tv_nsec = ( long ) ( fracPart * 1e9 );
      //  log ( Notice() , "Sleeping " , Integer ( uint32_t ( sleepTime.tv_sec ) ) , "s " , Integer ( uint32_t ( sleepTime.tv_nsec ) ) , "ns" );
      nanosleep ( &sleepTime, &returnTime );
    }

    //---
    std::vector<std::string> shellExpandPaths ( const std::string& aPath )
    {
      std::vector<std::string> lPaths;
      wordexp_t lSubstitutedPath;
      int code = wordexp ( aPath.c_str(), &lSubstitutedPath, WRDE_NOCMD );

      if ( code )
      {
        throw std::runtime_error ( "Failed expanding path: "+aPath );
      }

      for ( std::size_t i = 0 ; i != lSubstitutedPath.we_wordc ; i++ )
      {
        lPaths.push_back ( lSubstitutedPath.we_wordv[i] );
      }

      wordfree ( &lSubstitutedPath );
      return lPaths;
    }


    //---
    std::string shellExpandPath ( const std::string& aPath )
    {
      std::vector<std::string> lPaths = shellExpandPaths ( aPath );

      if ( lPaths.size() > 1 )
      {
        throw std::runtime_error ( "Failed to expand: multiple matches found" );
      }

      return lPaths[0];
    }

  } // namespace core
} // namespace swatch
