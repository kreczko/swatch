/**
 * @file    LinkStub.hpp
 * @author  Tom Williams
 * @date    June 2015
 */

#ifndef __SWATCH_PROCESSOR_LINKSTUB_HPP__
#define __SWATCH_PROCESSOR_LINKSTUB_HPP__


// C++ headers
#include <iosfwd>

// SWATCH headers
#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace system {


//! Structure that holds data required to build a link
class LinkStub : public swatch::core::AbstractStub {
public:

  LinkStub(const std::string& aId) :
    AbstractStub(aId) { }

  virtual ~LinkStub() { }


  //! Name of the source port
  std::string src;

  //! Name of the destination port
  std::string dst;
};


std::ostream& operator<<(std::ostream& aStream, const swatch::system::LinkStub& aStub);


} // namespace processor
} // namespace swatch


#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */