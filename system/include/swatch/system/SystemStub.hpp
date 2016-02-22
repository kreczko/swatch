/**
 * @file    SystemStub.hpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_SYSTEMSTUB_HPP__
#define __SWATCH_SYSTEM_SYSTEMSTUB_HPP__

// C++ headers
#include <iosfwd>
#include <map>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/LinkStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"


namespace swatch {
namespace system {


//! Structure that holds the data required to build a system
class SystemStub : public swatch::core::AbstractStub {
public:
  
  typedef std::map<uint32_t, std::vector<std::string> > FEDInputPortsMap;
  SystemStub(const std::string& aId) :
    AbstractStub(aId) { }

  SystemStub(const std::string& aId, const std::string& aCreator) :
    AbstractStub(aId),
    creator(aCreator)
  { }

  virtual ~SystemStub() { }

  //! Name of system class
  std::string creator;
  
  std::vector<std::string> excludedBoards;
  
  std::vector<swatch::system::CrateStub> crates;

  std::vector<swatch::processor::ProcessorStub> processors;

  std::vector<swatch::dtm::DaqTTCStub> daqttcs;

  std::vector<swatch::system::LinkStub> links;
  
  FEDInputPortsMap connectedFEDs;
};


std::ostream& operator<<(std::ostream& aStream, const swatch::system::SystemStub& aStub);


} // namespace system
} // namespace swatch

#endif /* __SWATCH_SYSTEM_SYSTEMSTUB_HPP__ */

