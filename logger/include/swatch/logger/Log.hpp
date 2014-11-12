/**
 * @file    Logger.hpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */

#ifndef SWATCH_LOGGER_LOG_HPP
#define SWATCH_LOGGER_LOG_HPP

#include <sstream>
#include <map>

// Macro Declaration
#define LOG(level) \
if (level > swatch::logger::Log::logThreshold()) ; \
else swatch::logger::Log().get(level)

namespace swatch {
namespace logger {

// Log, version 0.1: a simple logging class

enum LogLevel {
    kError,
    kWarning,
    kNotice,
    kInfo,
    kDebug,
    kDebug2,
    kDebug1,
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
    static const std::string& toString(LogLevel level);

protected:
    void push( LogLevel level, const std::string& source, const std::string& message );

    std::ostringstream os_;

private:
    Log(const Log&);
    Log& operator=(const Log&);

private:
    LogLevel messageLevel_;

    static LogLevel logThreshold_;

    static const std::map<LogLevel, std::string> logNames_;
};


} // swatch
} // logger

#endif /* SWATCH_LOGGER_LOG_HPP */