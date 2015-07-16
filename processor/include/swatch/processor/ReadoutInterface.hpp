
#ifndef SWATCH_PROCESSOR_READOUTINTERFACE_HPP
#define SWATCH_PROCESSOR_READOUTINTERFACE_HPP


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

//! Abstract class defining the readout component interface of a processor
class ReadoutInterface: public core::MonitorableObject {
protected:

    ReadoutInterface() : core::MonitorableObject( "readout" ) {}

public:

    virtual ~ReadoutInterface() {}
};


} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_READOUTINTERFACE_HPP */
