
#ifndef __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGER_HPP__
#define __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGER_HPP__


#include "swatch/system/DaqTTCManager.hpp"


namespace swatch {
namespace system {
namespace test {


class DummyAMC13Manager : public system::DaqTTCManager {
public:
    DummyAMC13Manager( const swatch::core::AbstractStub& aStub );

    virtual ~DummyAMC13Manager();
    
private:
    virtual void retrieveMetricValues();
};


} // namespace test
} // namespace system
} // namespace swatch


#endif	/* __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGER_HPP__ */

