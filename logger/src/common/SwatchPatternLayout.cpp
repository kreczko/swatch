#include "swatch/logger/SwatchPatternLayout.hpp"
#include "swatch/logger/ansi.hpp"

#include <log4cplus/helpers/loglog.h>
#include <iostream>

namespace swatch {
namespace logger {

SwatchPatternLayout::SwatchPatternLayout(const log4cplus::tstring& pattern) :
        log4cplus::PatternLayout(pattern) {
  PatternLayout::init(pattern, 0);
}

SwatchPatternLayout::SwatchPatternLayout(const log4cplus::helpers::Properties& properties) :
        log4cplus::PatternLayout(properties) {
}

SwatchPatternLayout::~SwatchPatternLayout() {

}

void SwatchPatternLayout::formatAndAppend(log4cplus::tostream& output,
    const log4cplus::spi::InternalLoggingEvent& event) {
  log4cplus::LogLevel lLoglevel = event.getLogLevel();
  const char* lColour = getColourForLogLevel(lLoglevel);
  output << lColour;
  PatternLayout::formatAndAppend(output, event);
  output << ansi::kReset;
}

void SwatchPatternLayout::init(const log4cplus::tstring& pattern, unsigned ndcMaxDepth) {
  PatternLayout::init(pattern, ndcMaxDepth);
}

const char* SwatchPatternLayout::getColourForLogLevel(log4cplus::LogLevel aLogLevel) const {
  using namespace log4cplus;
  switch (aLogLevel) {
  case FATAL_LOG_LEVEL:
    return ansi::kRed;
  case ERROR_LOG_LEVEL:
    return ansi::kLightRed;
  case WARN_LOG_LEVEL:
    return ansi::kYellow;
  case INFO_LOG_LEVEL:
    return ansi::kBlue;
  case DEBUG_LOG_LEVEL:
    return ansi::kCyan;
  case TRACE_LOG_LEVEL:
    return ansi::kMagenta;
  default:
    return "";
  }
}

}
}

