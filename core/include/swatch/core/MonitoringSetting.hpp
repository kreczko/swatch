/*
 * MetricSettings.hpp
 *
 *  Created on: 26 Oct 2015
 *      Author: kreczko
 */

#ifndef __SWATCH_CORE_MONITORINGSETTING_HPP__
#define __SWATCH_CORE_MONITORINGSETTING_HPP__
//#include "swatch/core/AbstractMetric.hpp"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <map>

namespace swatch {
namespace core {
namespace monitoring {

/**
 * Flag for deciding if failures of MonitorableObject will affect the parent.
 * If set to kENABLED they will (default behaviour) and if set to kNON_CRITICAL
 * they will not.
 */
enum Status {
  kEnabled,
  kNonCritical,
  kDisabled,
};

extern const std::map<std::string, Status> kStringToStatus;

std::ostream& operator<<(std::ostream& aStream, const monitoring::Status aMonStatus);

} // namespace monitoring

class MonitoringSetting {
public:
  MonitoringSetting(const std::string aId, const swatch::core::monitoring::Status aMonStatus);

//      boost::shared_ptr<AbstractMetricCondition>,
//      boost::shared_ptr<AbstractMetricCondition>,
//      );
    swatch::core::monitoring::Status getStatus() const;
    const std::string& getId() const;


    //! Returns metric's warning condition; NULL returned if metric doesn't have any warning condition
//    const AbstractMetricCondition* getWarningCondition() const;
//
//    //! Returns metric's error condition; NULL returned if metric doesn't have any error condition
//    const AbstractMetricCondition* getErrorCondition() const;


private:
    std::string mId;
    swatch::core::monitoring::Status mMonitoringStatus;
//    const boost::shared_ptr<AbstractMetricCondition> mErrorCondition_;
//    const boost::shared_ptr<AbstractMetricCondition> mWarnCondition;
};

typedef std::vector<MonitoringSetting> MonitoringSettings_t;
typedef boost::shared_ptr<MonitoringSetting> MonitoringSettingPtr_t;

} // namespace core
} // namespace swatch


#endif /* __SWATCH_CORE_MONITORINGSETTING_HPP__ */
