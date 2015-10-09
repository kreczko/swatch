
#ifndef __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__
#define __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__


#include "swatch/dtm/DaqTTCManager.hpp"


namespace swatch {
namespace dummy {


class DummyAMC13Driver;

class DummyAMC13Manager : public dtm::DaqTTCManager {
public:
    DummyAMC13Manager( const swatch::core::AbstractStub& aStub );

    virtual ~DummyAMC13Manager();
    
    DummyAMC13Driver& getDriver() { return *driver_;}
    
private:
    virtual void retrieveMetricValues();
    
    boost::scoped_ptr<DummyAMC13Driver> driver_;
};


} // namespace dummy
} // namespace swatch


#endif	/* __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__ */

