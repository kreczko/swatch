#include "swatch/logger/Log.hpp"
#include "swatch/logger/Logger.hpp"


// boost headers
#include <boost/assign.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/lexical_cast.hpp>
#include "boost/thread/thread.hpp"
#include "swatch/logger/ansi.hpp"

// C++ headers
#include <iostream>


namespace swatch {
namespace logger {

LogLevel Log::sLogThreshold = kInfo;

const char* const Log::kLogNames[] = {
    "FATAL",
    "ERROR",
    "WARNING",
    "NOTICE",
    "INFO",
    "DEBUG",
    "DEBUG1",
    "DEBUG2",
    "DEBUG3",
    "DEBUG4"
};


Log::Log() :
  mMessageLevel(LogLevel::kError) {
}

Log::Log(const Log&) :
  mMessageLevel(LogLevel::kError) {
}

Log::~Log() {
  if (mMessageLevel <= logThreshold()) {
    push(mMessageLevel, "swatch", mOSStream.str().c_str());
  }
}

std::ostringstream&
Log::get(LogLevel aLevel) {
  mMessageLevel = aLevel;
  return mOSStream;
}

void
Log::push(LogLevel aLevel, const std::string& aSource, const std::string& aMessage) {
  Logger::log(mMessageLevel, aMessage);
}

LogLevel& Log::logThreshold() {
  return sLogThreshold;
}

void
Log::setLogThreshold(LogLevel aLevel) {
  sLogThreshold = aLevel;
}


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
