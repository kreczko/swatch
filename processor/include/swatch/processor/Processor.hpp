/**
 * @file    Processor.hpp
 * @author  Alessandro Thea
 * @brief   Processor abstract interface
 * @date    1July 2014
 */

#ifndef __SWATCH_PROCESSOR_PROCESSOR_HPP__
#define __SWATCH_PROCESSOR_PROCESSOR_HPP__


// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/exception.hpp"
#include "swatch/processor/ProcessorStub.hpp"


namespace swatch {

namespace core {
class AbstractMetric;
class XParameterSet;
}

namespace processor {

class TTCInterface;
class ReadoutInterface;
class AlgoInterface;
class LinkInterface;

class Processor : public core::ActionableObject {
protected:
  Processor(const swatch::core::AbstractStub& );
public:
    virtual ~Processor();
 
    const ProcessorStub& getStub() const;

    /**
     * Return the board slot number
     * @return 
     */
    uint32_t getSlot() const;
    
    /**
     * Processor crate id getter
     * @return String with the name of the crate
     */
    const std::string& getCrateId() const;

    //! Constant corresponding to no slot being assigned
    static const uint32_t NoSlot;

    /**
     * Additional firmware information.
     * The string is meant to be informative for the user
     * @details [long description]
     * @return String containing additional firmware informations
     */
    virtual std::string firmwareInfo() const = 0; 

    TTCInterface& ttc();
    
    ReadoutInterface& readout();
    
    AlgoInterface& algo();
    
    LinkInterface& linkInterface();
    
    static const std::vector<std::string> defaultMetrics;

    static const std::vector<std::string> defaultMonitorableObjects;
    
protected:

    void Add( TTCInterface* aTTCInterface );
    void Add( ReadoutInterface* aReadoutInterface );
    void Add( AlgoInterface* aAlgoInterface );
    void Add( LinkInterface* aLinkInterface );

    //! Firmware version metric
    core::Metric<uint64_t>& metricFirmwareVersion_;
    
private:
  
    ProcessorStub stub_;

    uint32_t slot_;
    
    std::string crateId_;
    
    //! TTC control interface
    TTCInterface* ttc_;

    //! Readout control interface
    ReadoutInterface* readout_;

    //!
    AlgoInterface* algo_;

    LinkInterface* links_;

private:
    Processor( const Processor& other ); // non copyable
    Processor& operator=( const Processor& ); // non copyable
};


DEFINE_SWATCH_EXCEPTION(TTCInterfaceAlreadyDefined);
DEFINE_SWATCH_EXCEPTION(ReadoutInterfaceAlreadyDefined);
DEFINE_SWATCH_EXCEPTION(AlgoInterfaceAlreadyDefined);
DEFINE_SWATCH_EXCEPTION(LinkInterfaceAlreadyDefined);

}
}

#endif	/* __SWATCH_PROCESSOR_PROCESSOR_HPP__ */

