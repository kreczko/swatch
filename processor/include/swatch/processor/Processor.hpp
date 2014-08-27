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
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/AbstractInfo.hpp"
#include "swatch/processor/AbstractCtrl.hpp"
#include "swatch/processor/AbstractTTC.hpp"
#include "swatch/processor/AbstractChannel.hpp"


namespace swatch {
namespace processor {

/**
 *  A port class.
 */
class Processor : public core::Device {
public:
    Processor(const std::string& aId, const core::Arguments& aArguments = core::Arguments());
    virtual ~Processor();

    virtual uint32_t getSlot() const = 0;
    virtual const std::string& getCrateId() const = 0;

    static const uint32_t NoSlot;

    AbstractInfo* info() {
        return info_;
    }

    AbstractCtrl* ctrl() {
        return ctrl_;
    }

    AbstractTTC* ttc() {
        return ttc_;
    }

    std::vector<AbstractChannel*> getRx() {
        return rx_;
    }

    std::vector<AbstractChannel*> getTx() {
        return tx_;
    }

    Connection* connection() {
        return connection_;
    }
protected:
    AbstractInfo* info_;
    AbstractCtrl* ctrl_;
    AbstractTTC* ttc_;

    std::vector<AbstractChannel*> tx_;
    std::vector<AbstractChannel*> rx_;

    Connection* connection_;

};

}
}

#endif	/* _swatch_test_processor_hpp_ */

