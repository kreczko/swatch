/* 
 * File:   AbstractChannel.hpp
 * Author: mguilbau
 *
 * Created on 24 juillet 2014, 17:03
 */
#ifndef ABSTRACTCHANNEL_HPP
#define	ABSTRACTCHANNEL_HPP

// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Connection.hpp"
#include "swatch/processor/AbstractChanBuffer.hpp"
#include "swatch/processor/AbstractChanCtrl.hpp"

// C++ HEADERS

namespace swatch {
namespace processor {

class AbstractChannel {
protected:
    AbstractChannel(Connection* connection);

    Connection* connection() {
        return connection_;
    }

    AbstractChanCtrl* ctrl_;
    AbstractChanBuffer* buffer_;

public:
    virtual ~AbstractChannel();

    AbstractChanCtrl* getCtrl() {
        return ctrl_;
    }

    AbstractChanBuffer* getBuffer() {
        return buffer_;
    }

private:
    Connection* connection_;

};

}
}
#endif	/* ABSTRACTCHANNEL_HPP */

