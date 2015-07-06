/**
 * @file    CrateStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_CRATESTUB_HPP__
#define __SWATCH_SYSTEM_CRATESTUB_HPP__

// C++ Headers
#include <ostream>

// Swatch Headers
#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace system {

/**
 * @class CrateStub
 * @brief Struct to hold the data that is required to construct a swatch::system::Crate object
 */
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