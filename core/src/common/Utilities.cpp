#include "swatch/core/Utilities.hpp"

// C++
#include <cxxabi.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <cstdarg>

namespace swatch {
namespace core {

std::string
demangleName(const char* iMangledName) {
    int status = 0;
    char * demangled = abi::__cxa_demangle(iMangledName, 0, 0, &status);
    const std::string name(status == 0 ? demangled : "[UNKNOWN]");
    if ((status == 0) and (NULL != demangled)) {
        free(demangled);
    }
    return name;
}


std::string
anyToString(const boost::any& aAny) {
    std::stringstream ss;
    if (aAny.type() == typeid (int)) {
        ss << boost::any_cast<int>(aAny);
    } else if (aAny.type() == typeid (float)) {
        ss << boost::any_cast<int>(aAny);
    } else if (aAny.type() == typeid (double)) {
        ss << boost::any_cast<double>(aAny);
    } else if (aAny.type() == typeid (std::string)) {
        ss << boost::any_cast<std::string>(aAny);
    } else {
        ss << core::demangleName(aAny.type().name());
    }
    return ss.str();
}

std::string
strPrintf( const char* fmt, ... ) {
    char* ret;
    va_list ap;
    va_start(ap, fmt);
    vasprintf(&ret, fmt, ap);
    va_end(ap);
    std::string str(ret);
    free(ret);
    return str;
}
}

}
