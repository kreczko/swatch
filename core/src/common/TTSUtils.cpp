#include "swatch/core/TTSUtils.hpp"

namespace swatch {
namespace core {
namespace tts {

const std::string kWarningStr="Warning Overflow (0x1)";
const std::string kOutOfSyncStr="Out of Sync (0x2)";
const std::string kBusyStr="Busy (0x4)";
const std::string kReadyStr="Ready (0x8)";
const std::string kErrorStr="Error (0xc)";
const std::string kDisconnectedStr="Disconnected (0xf)";
const std::string kUnknownStr="Unknown";

const std::string& codeToString( uint32_t aCode ) {
    switch ( aCode ) {
        case kWarning:
            return kWarningStr;
        case kOutOfSync:
            return kOutOfSyncStr;
        case kBusy:
            return kBusyStr;
        case kReady:
            return kReadyStr;
        case kError:
            return kErrorStr;
        case kDisconnected:
            return kDisconnectedStr;
        default:
            return kUnknownStr;
    }
}

} // namespace tts
} // namespace core
} // namespace swatch
