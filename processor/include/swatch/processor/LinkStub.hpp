/**
 * @file    LinkStub.hpp
 * @author  Tom Williams
 * @date    June 2015
 */

#ifndef __SWATCH_PROCESSOR_LINKSTUB_HPP__
#define __SWATCH_PROCESSOR_LINKSTUB_HPP__


// C++ Headers
#include <iosfwd>

// Swatch Headers
#include "swatch/core/AbstractStub.hpp"

namespace swatch {
namespace processor {


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


//typedef xdata::Bag<LinkStub> LinkBag;


std::ostream& operator<<(std::ostream& os, const swatch::processor::LinkStub& pd);


} // namespace processor
} // namespace swatch


#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */