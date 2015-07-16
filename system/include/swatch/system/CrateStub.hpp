/**
 * @file    CrateStub.hpp
 * @author  Alessandro Thea
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_CRATESTUB_HPP__
#define __SWATCH_SYSTEM_CRATESTUB_HPP__

// C++ headers
#include <iosfwd>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"


namespace swatch {
namespace system {

//! Structure that holds the data required to build a crate
class CrateStub : public swatch::core::AbstractStub {
public:

  CrateStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~CrateStub() { }

  //! Description of the crate
  std::string description;

  //! Geo location of the crate
  std::string location;
};


std::ostream& operator<<(std::ostream& os, const swatch::system::CrateStub& sd);

} // namespace system
} // namespace swatch



#endif /* __SWATCH_SYSTEM_CRATESTUB_HPP__ */