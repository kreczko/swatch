#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h" 
#include <log4cplus/configurator.h>

namespace l4c = log4cplus;

int main(int argc, char const *argv[])
{
  l4c::initialize();
  l4c::BasicConfigurator config;
  config.configure();

    l4c::Logger logger = l4c::Logger::getInstance("alogger");
  
    LOG4CPLUS_WARN(logger, "amessage");
    
    return 0;
}