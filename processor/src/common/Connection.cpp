// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Connection.hpp"

// C++ HEADERS
#include <cxxabi.h>
#include <stdlib.h>

namespace swatch {
namespace processor {

std::string Connection::demangleName(const char* iMangledName) {
    int status = 0;
    char * demangled = abi::__cxa_demangle(iMangledName, 0, 0, &status);
    const std::string name(status == 0 ? demangled : "[UNKNOWN]");
    if ((status == 0) and (NULL != demangled)) {
        free(demangled);
    }
    return name;

}

}
}
