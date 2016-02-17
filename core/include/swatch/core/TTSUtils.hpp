#ifndef __SWATCH_CORE_TTSUTILS_HPP__
#define __SWATCH_CORE_TTSUTILS_HPP__

// C++ Headers
#include <stdint.h>
#include <string>
#include <ostream>

namespace swatch {
namespace core {
namespace tts {

enum State {
    kWarning = 0x1,
    kOutOfSync = 0x2,
    kBusy = 0x4,
    kReady = 0x8,
    kError = 0xc,
    kDisconnected = 0xf,
    kUnknown = 0xff
};

std::ostream& operator<<(std::ostream& oStream, const State& aState);

} // namespace tts
} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_TTSUTILS_HPP__ */