#include "log4cplus/version.h"
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h" 
// #include <log4cplus/configurator.h>
#include "swatch/logger/log4cpluscfg.hpp"

namespace l4c = log4cplus;

int main(int argc, char const *argv[])
{
  #if LOG4CPLUS_VERSION>LOG4CPLUS_MAKE_VERSION(1, 0, 4)
    l4c::initialize();
  #endif

  l4c::BasicConfigurator config;
  config.configure();

    l4c::Logger logger = l4c::Logger::getInstance("alogger");
  
    LOG4CPLUS_DEBUG(logger, "amessage");
    LOG4CPLUS_WARN(logger, "amessage");
    return 0;
}