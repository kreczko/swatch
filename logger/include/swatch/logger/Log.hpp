/**
 * @file    Logger.hpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    June 2014
 * @deprecated
 */

#ifndef __SWATCH_LOGGER_LOG_HPP__
#define __SWATCH_LOGGER_LOG_HPP__

#include <sstream>
#include <map>
#include <vector>

// Macro Declaration
#define LOG(level) \
if (level > ::swatch::logger::Log::logThreshold()) ; \
else ::swatch::logger::Log().get(level)

namespace swatch {
namespace logger {

// Log, version 0.1: a simple logging class

enum LogLevel {
    kFatal = 0,
    kError,
    kWarning,
    kNotice,
    kInfo,
    kDebug,
    kDebug1,
    kDebug2,
    kDebug3,
    kDebug4
};

class Log {
public:
    Log();
    virtual ~Log();
    std::ostringstream& get(LogLevel level = kInfo);
public:
    static LogLevel& logThreshold();
    static void setLogThreshold( LogLevel level ); 
    // static const std::string& toString(LogLevel level);

protected:
    void push( LogLevel level, const std::string& source, const std::string& message );

    std::ostringstream os_;

private:
    Log(const Log&);
    Log& operator=(const Log&);

private:
    LogLevel messageLevel_;

    static LogLevel sLogThreshold;

    static const char* const kLogNames[];
};

template<typename T>
extern std::string vecFmt(const std::vector<T>& aVec);

template<typename T>
extern std::string shortVecFmt(const std::vector<T>& aVec);


} // swatch
} // logger

#endif /* __SWATCH_LOGGER_LOG_HPP__ */
