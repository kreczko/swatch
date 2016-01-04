#include "swatch/logger/Logger.hpp"
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/spi/factory.h>

#include "swatch/logger/SwatchPatternLayout.hpp"

namespace swatch {
namespace logger {

bool Logger::sInitialised = false;
// needs absolute path
std::string Logger::sConfigFile = "log4cplus.properties";

log4cplus::Logger Logger::getInstance(const log4cplus::tstring& aName) {
  using namespace log4cplus;
  if (!sInitialised) {
    {
      // this shoud probably have a lock
      ::log4cplus::initialize();
      // register our stuff
      Logger::registerCustomLoggingClasses();
      PropertyConfigurator::doConfigure(Logger::sConfigFile);
      Logger::sInitialised = true;
    }
  }
  return log4cplus::Logger::getInstance(aName);

}

void Logger::log(LogLevel aLevel, const std::string& aMessage) {
  log4cplus::Logger logger = Logger::getInstance("swatch");
  switch (aLevel) {
  case kFatal:
    LOG4CPLUS_FATAL(logger, aMessage);
    break;
  case kError:
    LOG4CPLUS_ERROR(logger, aMessage);
    break;
  case kInfo:
    LOG4CPLUS_INFO(logger, aMessage);
    break;
  case kNotice:
    LOG4CPLUS_INFO(logger, "[log level kNotice is deprecated] " << aMessage);
    break;
  case kDebug:
    LOG4CPLUS_DEBUG(logger, aMessage);
    break;
  case kWarning:
    LOG4CPLUS_WARN(logger, aMessage);
    break;
  default:
    LOG4CPLUS_TRACE(logger, aMessage);
    break;
  }
}

void Logger::registerCustomLoggingClasses() {
  using namespace log4cplus;
  // register all custom layouts
  spi::LayoutFactoryRegistry& reg = spi::getLayoutFactoryRegistry();
  LOG4CPLUS_REG_PRODUCT (reg, "swatch::logger::", SwatchPatternLayout, swatch::logger::, spi::LayoutFactory);
}

}
}
