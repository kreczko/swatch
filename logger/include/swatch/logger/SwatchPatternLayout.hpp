/*
 * SwatchPatternLayout.hpp
 * Custom PatternLayout for log4cplus
 *
 *  Created on: 16 Nov 2015
 *      Author: kreczko
 *
 *      https://github.com/log4cplus/log4cplus/blob/master/src/layout.cxx
 */

#include <log4cplus/layout.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/spi/loggingevent.h>

#ifndef __SWATCH_LOGGER_SWATCHPATTERNLAYOUT_HPP___
#define __SWATCH_LOGGER_SWATCHPATTERNLAYOUT_HPP___


namespace swatch {
namespace logger {


class LOG4CPLUS_EXPORT SwatchPatternLayout: public log4cplus::PatternLayout {

public:
  SwatchPatternLayout(const log4cplus::tstring& pattern);
  SwatchPatternLayout(const log4cplus::helpers::Properties& properties);
  ~SwatchPatternLayout();

  virtual void formatAndAppend(log4cplus::tostream& output, const log4cplus::spi::InternalLoggingEvent& event);

protected:
        void init(const log4cplus::tstring& pattern, unsigned ndcMaxDepth = 0);

private:
      // Disallow copying of instances of this class
  SwatchPatternLayout(const SwatchPatternLayout&);
  SwatchPatternLayout& operator=(const SwatchPatternLayout&);

  const char* getColourForLogLevel(log4cplus::LogLevel aLogLevel) const;

};

}
}
#endif /* __SWATCH_LOGGER_SWATCHPATTERNLAYOUT_HPP___ */
