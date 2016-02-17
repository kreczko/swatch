#include "swatch/core/TTSUtils.hpp"

#include <iomanip>
namespace swatch {
namespace core {
namespace tts {

std::ostream& 
operator<<(std::ostream& oStream, const State& aState) {
  switch (aState) {
    case kWarning:
      return (oStream << "Warning Overflow (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    case kOutOfSync:
      return (oStream << "Out of Sync (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    case kBusy:
      return (oStream << "Busy (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    case kReady:
      return (oStream << "Ready (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    case kError:
      return (oStream << "Error (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    case kDisconnected:
      return (oStream << "Disconnected (0x" << std::hex <<  static_cast<uint32_t>(aState) << ")");
    default:
      return (oStream << "Unknown");
  }
}


} // namespace tts
} // namespace core
} // namespace swatch
