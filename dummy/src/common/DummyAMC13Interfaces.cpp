
#include "swatch/dummy/DummyAMC13Interfaces.hpp"


// SWATCH headers
#include "swatch/dummy/DummyAMC13Driver.hpp"


namespace swatch {
namespace dummy {


AMC13BackplaneDaqPort::AMC13BackplaneDaqPort(uint32_t aSlot, DummyAMC13Driver& aDriver) : 
  dtm::AMCPort(aSlot),
  mDriver(aDriver)
{
}

AMC13BackplaneDaqPort::~AMC13BackplaneDaqPort()
{
}

void AMC13BackplaneDaqPort::retrieveMetricValues()
{
}


//--------------------------------------------------------------------

AMC13EventBuilder::AMC13EventBuilder(DummyAMC13Driver& aDriver) : 
  mDriver(aDriver)
{
}

AMC13EventBuilder::~AMC13EventBuilder()
{
}

void AMC13EventBuilder::retrieveMetricValues()
{
}


//--------------------------------------------------------------------

AMC13SLinkExpress::AMC13SLinkExpress(uint32_t aSfpId, DummyAMC13Driver& aDriver) :
  dtm::SLinkExpress(aSfpId),
  mDriver(aDriver)
{
}

AMC13SLinkExpress::~AMC13SLinkExpress()
{
}

void AMC13SLinkExpress::retrieveMetricValues()
{
}


//--------------------------------------------------------------------

AMC13TTC::AMC13TTC(DummyAMC13Driver& aDriver) : 
mDriver(aDriver)
{
}

AMC13TTC::~AMC13TTC()
{
}

void AMC13TTC::retrieveMetricValues()
{
}


} // namespace dummy
} // namespace swatch
