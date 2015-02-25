#include "swatch/logger/Log.hpp"


// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>

// C++ Headers
#include <iostream>


namespace swatch {
namespace logger {

namespace ansi {
  const char *const kEsc       = "\033[";
  const char *const kReset     = "\033[0m";
  const char *const kBlack     = "\033[30m";
  const char *const kRed       = "\033[31m";
  const char *const kGreen     = "\033[32m";
  const char *const kYellow    = "\033[33m";
  const char *const kBlue      = "\033[34m";

  const char *const kMagenta   = "\033[35m";
  const char *const kCyan      = "\033[36m";
  const char *const kWhite     = "\033[37m";

  const char *const kBckBlack  = "\033[40m";
  const char *const kBckRed    = "\033[41m";
  const char *const kBckGreen  = "\033[42m";
  const char *const kBckYellow = "\033[43m";
  const char *const kBckBlue   = "\033[44m";
  const char *const kBckMagenta= "\033[45m";
  const char *const kBckCyan   = "\033[46m";
  const char *const kBckWhite  = "\033[47m";

  const char *const kUnderline = "\033[4m";
  const char *const kBlink     = "\033[5m";
  const char *const kBright    = "\033[1m";
  const char *const kDark      = "\033[2m";
}

LogLevel Log::logThreshold_ = kInfo;

// const std::map<LogLevel, std::string> Log::logNames_ = boost::assign::map_list_of
//         (kError, "ERROR")
//         (kWarning, "WARNING")
//         (kNotice, "NOTICE")
//         (kInfo, "INFO")
//         (kDebug, "DEBUG")
//         (kDebug2, "DEBUG2")
//         (kDebug1, "DEBUG1")
//         (kDebug3, "DEBUG3")
//         (kDebug4, "DEBUG4");

// const std::map<LogLevel, std::string> logColors = boost::assign::map_list_of
//         (kError, "ERROR")
//         (kWarning, "WARNING")
//         (kNotice, "NOTICE")
//         (kInfo, "INFO")
//         (kDebug, "DEBUG")
//         (kDebug2, "DEBUG2")
//         (kDebug1, "DEBUG1")
//         (kDebug3, "DEBUG3")
//         (kDebug4, "DEBUG4");

const char* Log::logNames_[] = {
    "ERROR",
    "WARNING",
    "NOTICE",
    "INFO",
    "DEBUG",
    "DEBUG2",
    "DEBUG1",
    "DEBUG3",
    "DEBUG4"
};

const char* logColors[] = {
    ansi::kRed,
    ansi::kYellow,
    ansi::kGreen,
    ansi::kBlue,
    ansi::kCyan,
    ansi::kCyan,
    ansi::kCyan,
    ansi::kCyan,
    ansi::kCyan
    };

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
    // fprintf(stderr, "%s %-7s |  %s", source.c_str(), toString(level).c_str(), message.c_str());
    fprintf(stderr, "%s%s %-7s |  %s%s", logColors[level], source.c_str(), logNames_[level], message.c_str(),ansi::kReset);
    fflush(stderr);
}

LogLevel& Log::logThreshold() {
    return logThreshold_;
}

void
Log::setLogThreshold(LogLevel level) {
    logThreshold_ = level;
}


// const std::string&
// Log::toString(LogLevel level) {
//     std::map<LogLevel, std::string>::const_iterator it = logNames_.find(level);
//     return it->second;
// }

template <typename T>
std::string vecFmt(const std::vector<T>& aVec)
{
  std::ostringstream oss;
  oss << "[";

  for(typename std::vector<T>::const_iterator it=aVec.begin(); it != aVec.end(); it++)
    oss << *it << ",";
  oss.seekp(oss.tellp()-1);
  oss << "]";

  return oss.str();
}


template <typename T>
std::string shortVecFmt(const std::vector<T>& aVec)
{
  if (aVec.size() == 0)
    return "[]";
  else if (aVec.size() == 1)
    return "[" + boost::lexical_cast<std::string>(aVec.at(0)) + "]";

  std::ostringstream oss;
  oss << "[";

  T begin = aVec.at(0);
  T end   = begin;
  for(typename std::vector<T>::const_iterator it=aVec.begin()+1; it != aVec.end(); it++)
  {
    if((*it) == (end + 1))
    {
      end = *it;
      continue;
    }

    if (begin == end)
      oss << begin << ",";
    else
      oss << begin << "-" << end << ",";

    begin = *it;
    end = *it;
  }

  if (begin == end)
    oss << begin << ",";
  else
    oss << begin << "-" << end << ",";

  // Replace final "," with a "]"
  oss.seekp(oss.tellp()-1);
  oss << "]";

  return oss.str();
}

} // logger
} // swatch


template std::string swatch::logger::vecFmt<uint32_t>(const std::vector<uint32_t>& aVec);
template std::string swatch::logger::shortVecFmt<uint32_t>(const std::vector<uint32_t>& aVec);
