
#ifndef __SWATCH_DUMMY_DUMMYAMC13INTERFACES_HPP__
#define __SWATCH_DUMMY_DUMMYAMC13INTERFACES_HPP__


#include "swatch/dtm/AMCPort.hpp"
#include "swatch/dtm/EVBInterface.hpp"
#include "swatch/dtm/SLinkExpress.hpp"
#include "swatch/dtm/TTCInterface.hpp"


namespace swatch {
namespace dummy {

class DummyAMC13Driver;


class AMC13BackplaneDaqPort : public dtm::AMCPort {
public:
  AMC13BackplaneDaqPort(uint32_t aSlot, DummyAMC13Driver& aDriver);
  ~AMC13BackplaneDaqPort();

private:
  void retrieveMetricValues();
    
  DummyAMC13Driver& mDriver;
};


class AMC13EventBuilder : public dtm::EVBInterface {
public:
  AMC13EventBuilder(DummyAMC13Driver& aDriver);
  ~AMC13EventBuilder();
    
private:
  void retrieveMetricValues();
    
  DummyAMC13Driver& mDriver;
};


class AMC13SLinkExpress : public dtm::SLinkExpress {
public:
  AMC13SLinkExpress(uint32_t aSfpID, DummyAMC13Driver& aDriver);
  ~AMC13SLinkExpress();

private:
  void retrieveMetricValues();
    
  DummyAMC13Driver& mDriver;
};


class AMC13TTC : public dtm::TTCInterface {
public:
  AMC13TTC(DummyAMC13Driver& aDriver);
  ~AMC13TTC();

private:
  void retrieveMetricValues();
    
  DummyAMC13Driver& mDriver;
};


} // namespace dummy
} // namespace swatch


#endif	/* __SWATCH_DUMMY_DUMMYAMC13MANAGER_HPP__ */

