/**
 * @file    AbstractChannel.hpp
 * @author  Maxime Guilbaud
 * @brief   Brief description
 * @date    24/07/2014
 */

#ifndef SWATHC_PROCESSOR_ABSTRACTCHANNEL_HPP
#define	SWATHC_PROCESSOR_ABSTRACTCHANNEL_HPP

// Swatch Headers
#include "swatch/processor/AbstractChanBuffer.hpp"
#include "swatch/processor/AbstractChanCtrl.hpp"

// C++ HEADERS

namespace swatch {
namespace processor {

class AbstractChannel : public Component {
protected:
    AbstractChannel(Connection* connection) : Component( connection ) {}
            
    AbstractChanCtrl* ctrl_;
    AbstractChanBuffer* buffer_;

public:
    virtual ~AbstractChannel() {}

    AbstractChanCtrl* ctrl() {
        return ctrl_;
    }

    AbstractChanBuffer* buffer() {
        return buffer_;
    }
};

}
}
#endif	/* SWATHC_PROCESSOR_ABSTRACTCHANNEL_HPP */

