#include "swatch/logger/Log.hpp"


// Boost Headers
#include <boost/assign.hpp>

// C++ Headers
#include <iostream>


namespace swatch {
namespace logger {

LogLevel Log::logThreshold_ = kInfo;

const std::map<LogLevel, std::string> Log::logNames_ = boost::assign::map_list_of
        (kError, "ERROR")
        (kWarning, "WARNING")
        (kNotice, "NOTICE")
        (kInfo, "INFO")
        (kDebug, "DEBUG")
        (kDebug2, "DEBUG2")
        (kDebug1, "DEBUG1")
        (kDebug3, "DEBUG3")
        (kDebug4, "DEBUG4");

Log::Log() {
}

Log::Log(const Log&) {
}

Log::~Log() {
    if (messageLevel_ <= logThreshold()) {
        os_ << std::endl;
        push(messageLevel_, "swatch", os_.str().c_str());
    }
}

//Log&
//Log::operator=(const Log&) {
//}

std::ostringstream&
Log::get(LogLevel level) {
    messageLevel_ = level;
    return os_;
}

void
Log::push(LogLevel level, const std::string& source, const std::string& message) {
    fprintf(stderr, "%s %-7s |  %s", source.c_str(), toString(level).c_str(), message.c_str());
    fflush(stderr);
}

LogLevel& Log::logThreshold() {
    return logThreshold_;
}

void
Log::setLogThreshold(LogLevel level) {
    logThreshold_ = level;
}


const std::string&
Log::toString(LogLevel level) {
    std::map<LogLevel, std::string>::const_iterator it = logNames_.find(level);
    return it->second;
}



} // swatch
} // logger