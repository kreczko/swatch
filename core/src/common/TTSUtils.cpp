#include "swatch/core/TTSUtils.hpp"

namespace swatch {
namespace core {
namespace tts {

const std::string kWarningStr="Warning Overflow";
const std::string kOutOfSyncStr="Out of Sync";
const std::string kBusyStr="Busy";
const std::string kReadyStr="Ready";
const std::string kErrorStr="Error";
const std::string kDisconnectedStr="Disconnected";
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
