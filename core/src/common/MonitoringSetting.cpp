#include "swatch/core/MonitoringSetting.hpp"
#include <boost/assign.hpp>

namespace swatch {
namespace core {

namespace monitoring {

extern const std::map<std::string, Status> StringToStatus = boost::assign::map_list_of("enabled", kEnabled)(
    "non-critical", kNonCritical)("disabled", kDisabled);

std::ostream& operator<<(std::ostream& aStream, const monitoring::Status aMonStatus) {
  switch (aMonStatus) {
  case kEnabled:
    aStream << "enabled";
    break;
  case kNonCritical:
    aStream << "non-critical";
    break;
  case kDisabled:
    aStream << "disabled";
    break;
  default:
    aStream << "enabled";
    break;
  }
  return aStream;
}
}

MonitoringSetting::MonitoringSetting(const std::string aId, const swatch::core::monitoring::Status aMonStatus) :
        mId(aId),
        mMonitoringStatus(aMonStatus) {

}

swatch::core::monitoring::Status MonitoringSetting::getStatus() const {
  return mMonitoringStatus;
}

const std::string& MonitoringSetting::getId() const {
  return mId;
}

} // namespace core
} // namespace swatch
