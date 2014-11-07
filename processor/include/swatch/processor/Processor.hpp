/* 
 * File:   Processor.hpp
 * Author: ale
 *
 * Created on July 11, 2014, 10:55 AM
 */

#ifndef _swatch_test_processor_hpp_
#define	_swatch_test_processor_hpp_

// Swatch Headers
#include "swatch/core/Device.hpp"
#include "InputChannel.hpp"

namespace swatch {

namespace core {
class ParameterSet;
}

namespace processor {

class Controls;
class TTCInterface;
class InputChannel;
class OutputChannel;
class ReadoutInterface;
class AlgoInterface;

/**
 *  A port class.
 */
class Processor : public core::Device {
public:
    Processor(const std::string& aId, const core::ParameterSet& params = core::ParameterSet());
    virtual ~Processor();

    /**
     * Return the board slot number
     * @return 
     */
    virtual uint32_t getSlot() const = 0;
    
    /**
     * Returns the Processor crate id
     * @return 
     */
    virtual const std::string& getCrateId() const = 0;

    static const uint32_t NoSlot;

    
    Controls* ctrl();

    TTCInterface* ttc();
    
    ReadoutInterface* readout();
    
    AlgoInterface* algo();

    /**
     * Returns the vector of input channel pointers 
     * @return vector of input channel pointers 
     */
    const std::vector<InputChannel*>& inputChannels() const;

    InputChannel* inputChannel(uint32_t i);

    /**
     * Returns the vector of output channel pointers
     * @return output channel pointers
     */
    const std::vector<OutputChannel*>& outputChannels() const;

    OutputChannel* outputChannel(uint32_t i);

    // Basic control methods

    virtual std::set<std::string> getModes() const = 0;

    virtual void reset( const std::string& mode ) = 0;


protected:

    //! Master control interface
    Controls* ctrl_;

    //! TTC control interface
    TTCInterface* ttc_;

    //! Readout control interface
    ReadoutInterface* readout_;
    
    //!
    AlgoInterface* algo_;

    //! 
    std::vector<InputChannel*> inputChannels_;
    
    //!
    std::vector<OutputChannel*> outputChannels_;

    /**
     * @brief Assess whether the pre-conditions to Halt are satisfied.
     * @details Runs the checks to guarantee that the Halt transition can take place.
     * @return True if the Processor can be Halted.
     */
    virtual bool canHalt();

    /**
     * Perform configuration action
     * @param params
     */
    virtual void doHalt(const core::ParameterSet& params = core::ParameterSet());

    /**
     * @brief Assess whether the pre-conditions to Configure are satisfied.
     * @details Runs the checks to guarantee that the Configure transition can take place.
     * @return True if the Processor can be configured.
     */
    virtual bool canConfigure();

    /**
     * @brief Execute the configure transition
     * @details [long description]
     * 
     * @param params [description]
     */
    virtual void doConfigure(const core::ParameterSet& params = core::ParameterSet());

};

}
}

#endif	/* _swatch_test_processor_hpp_ */

