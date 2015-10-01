/* 
 * File:   DummyAMC13Service.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 9:18 PM
 */

#ifndef __SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__
#define __SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__


#include "swatch/system/DaqTTCManager.hpp"


namespace swatch {
namespace system {
namespace test {

class DummyAMC13Driver;

class DummyAMC13Manager : public system::DaqTTCManager {
public:
    DummyAMC13Manager( const swatch::core::AbstractStub& aStub );

    virtual ~DummyAMC13Manager();
    
    DummyAMC13Driver& getDriver() { return *driver_;}
    
private:
    virtual void retrieveMetricValues();
    
    boost::scoped_ptr<DummyAMC13Driver> driver_;
};

} // namespace test
} // namespace system
} // namespace swatch


#endif	/* __SWATCH_SYSTEM_DUMMYAMC13MANAGER_HPP__ */

