/* 
 * File:   SLinkExpress.hpp
 * Author: ale
 *
 * Created on October 1, 2015, 3:12 PM
 */

#ifndef __SWATCH_AMC13_SLINK_HPP__
#define	__SWATCH_AMC13_SLINK_HPP__

#include "swatch/core/MonitorableObject.hpp"
#include "swatch/dtm/SLinkExpress.hpp"

namespace amc13 {
  class AMC13;
} // namespace amc13

namespace swatch {
namespace amc13 {

class SLinkExpress : public dtm::SLinkExpress {
public:
  SLinkExpress( uint32_t aSfpID, ::amc13::AMC13& aDriver );
  virtual ~SLinkExpress();

protected:
    virtual void retrieveMetricValues();

private:
  ::amc13::AMC13& mDriver;

};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_SLINK_HPP__ */

