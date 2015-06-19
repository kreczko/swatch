/**
 * @file    LinkStub.hpp
 * @author  Tom Williams
 * @date    June 2015
 */

#ifndef __SWATCH_PROCESSOR_LINKSTUB_HPP__
#define __SWATCH_PROCESSOR_LINKSTUB_HPP__


// C++ Headers
#include <iosfwd>
 
// XDAQ Headers
#include "xdata/Bag.h"
#include "xdata/String.h"


namespace swatch {
namespace processor {


//! Structure that holds data required to build a link
struct LinkStub {

    void registerFields(xdata::Bag<LinkStub> *bag) {
        bag->addField("name", &name);
        bag->addField("src", &src);
        bag->addField("dst", &dst);
    }

    //! Name of the link
    xdata::String  name;

    //! Name of the source port
    xdata::String  src;
    
    //! Name of the destination port
    xdata::String  dst;
};


typedef xdata::Bag<LinkStub> LinkBag;


std::ostream& operator<<(std::ostream& os, const swatch::processor::LinkStub& pd );


} // namespace processor
} // namespace swatch


#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */