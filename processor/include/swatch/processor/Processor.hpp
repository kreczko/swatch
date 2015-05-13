/**
 * @file    Processor.hpp
 * @author  Alessandro Thea
 * @brief   Processor abstract interface
 * @date    1July 2014
 */

#ifndef __SWATCH_TEST_PROCESSOR_HPP__
#define	__SWATCH_TEST_PROCESSOR_HPP__

// Swatch Headers
#include "swatch/core/ActionableObject.hpp"

namespace swatch {

namespace core {
class XParameterSet;
}

namespace processor {

class TTCInterface;
class ReadoutInterface;
class AlgoInterface;
class Device;

/**
 *  @class Processor
 */
class Processor : public core::ActionableObject {
protected:
  Processor(const std::string& aId, const core::XParameterSet& params = core::XParameterSet());
public:
    virtual ~Processor();

    /**
     * Return the board slot number
     * @return 
     */
    virtual uint32_t getSlot() const = 0;
    
    /**
     * Processor crate id getter
     * @return String with the name of the crate
     */
    virtual const std::string& getCrateId() const = 0;

    //! Constant corresponing to no slot being assigned
    static const uint32_t NoSlot;

    /**
     * Firmware version getter
     * @return 64 bit word
     */
    virtual uint64_t firmwareVersion() const = 0;

    /**
     * Additional firmware information.
     * The string is meant to be informative for the user
     * @details [long description]
     * @return String containing additional firmware informations
     */
    virtual std::string firmwareInfo() const = 0; 

    TTCInterface* ttc();
    
    ReadoutInterface* readout();
    
    AlgoInterface* algo();

    Device* device();

protected:

    //! TTC control interface
    TTCInterface* ttc_;

    //! Readout control interface
    ReadoutInterface* readout_;
    
    //!
    AlgoInterface* algo_;

    Device* device_;


};

}
}

#endif	/* __SWATCH_TEST_PROCESSOR_HPP__ */

