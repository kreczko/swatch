/* 
 * File:   AMCLinkCollection.hpp
 * Author: ale
 *
 * Created on October 5, 2015, 4:57 PM
 */

#ifndef __SWATCH_AMC13_AMCLINKCOLLECTION_HPP__
#define	__SWATCH_AMC13_AMCLINKCOLLECTION_HPP__

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace amc13 {

class AMCPort;

class AMCPortCollection : public core::MonitorableObject {
public:
  AMCPortCollection();
  virtual ~AMCPortCollection();

  typedef std::deque<AMCPort*> AMCPortDeque_t;

  void addPort( AMCPort* aAMCPort );
  
protected:

  virtual void retrieveMetricValues() {};


private:
    AMCPortDeque_t mAMCPorts;
};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_AMCLINKCOLLECTION_HPP__ */

