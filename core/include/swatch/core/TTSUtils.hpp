#ifndef __SWATCH_CORE_TTSUTILS_HPP__
#define __SWATCH_CORE_TTSUTILS_HPP__

#include <stdint.h>
#include <string>


namespace swatch {
namespace core {
namespace tts {

enum State {
    kWarning = 0x1,
    kOutOfSync = 0x2,
    kBusy = 0x4,
    kReady = 0x8,
    kError = 0xc,
    kDisconnected = 0xf
};


extern const std::string kWarningStr;
extern const std::string kOutOfSyncStr;
extern const std::string kBusyStr;
extern const std::string kReadyStr;
extern const std::string kErrorStr;
extern const std::string kDisconnectedStr;
extern const std::string kUnknownStr;

const std::string& codeToString( uint32_t aCode );

} // namespace tts
} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_TTSUTILS_HPP__ */