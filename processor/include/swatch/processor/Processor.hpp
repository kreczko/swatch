/**
 * @file    Processor.hpp
 * @author  Alessandro Thea
 * @brief   Processor abstract interface
 * @date    1July 2014
 */

#ifndef __SWATCH_TEST_PROCESSOR_HPP__
#define	__SWATCH_TEST_PROCESSOR_HPP__

// Swatch Headers
#include "swatch/core/Device.hpp"
#include "swatch/core/ActionHandler.hpp"

namespace swatch {

namespace core {
class XParameterSet;
}

namespace processor {

class Controls;
class TTCInterface;
class ReadoutInterface;
class AlgoInterface;

/**
 *  A port class.
 */
class Processor : public core::Device, public core::ActionHandler {
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
    uint64_t firmwareVersion() const;

    /**
     * Additional firmware infos
     * @details [long description]
     * @return [description]
     */
    const std::string firmwareInfo() const; 


    Controls* ctrl();

    TTCInterface* ttc();
    
    ReadoutInterface* readout();
    
    AlgoInterface* algo();

protected:

    //! Master control interface
    Controls* ctrl_;

    //! TTC control interface
    TTCInterface* ttc_;

    //! Readout control interface
    ReadoutInterface* readout_;
    
    //!
    AlgoInterface* algo_;

};

}
}

#endif	/* __SWATCH_TEST_PROCESSOR_HPP__ */

