/**
 * @file    SystemStub.hpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_SYSTEMSTUB_HPP__
#define __SWATCH_SYSTEM_SYSTEMSTUB_HPP__

// C++ headers
#include <iosfwd>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/LinkStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/system/DaqTTCStub.hpp"


namespace swatch {
namespace system {


//! Structure that holds the data required to build a system
class SystemStub : public swatch::core::AbstractStub {
public:
  SystemStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~SystemStub() { }

  //! Description of the crate
  std::string creator;

  std::vector<swatch::system::CrateStub> crates;

  std::vector<swatch::processor::ProcessorStub> processors;

  std::vector<swatch::system::DaqTTCStub> daqttcs;

  std::vector<swatch::processor::LinkStub> links;
};


std::ostream& operator<<(std::ostream& os, const swatch::system::SystemStub& sd);


} // namespace system
} // namespace swatch

#endif /* __SWATCH_SYSTEM_SYSTEMSTUB_HPP__ */
