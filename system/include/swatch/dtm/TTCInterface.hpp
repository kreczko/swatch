/* 
 * File:   TTCInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:20 PM
 */

#ifndef __SWATCH_DTM_TTCINTERFACE_HPP___
#define	__SWATCH_DTM_TTCINTERFACE_HPP___

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace dtm {

class TTCInterface : public core::MonitorableObject {
protected:
    TTCInterface();
public:
    virtual ~TTCInterface( );
    
};

    
} // namespace amc13
} // namespace swatch 



#endif	/* __SWATCH_DTM_TTCINTERFACE_HPP___ */

