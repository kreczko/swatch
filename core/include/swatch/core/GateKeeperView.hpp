/**
 * @file    GateKeeperView.hpp
 * @author  Luke Kreczko
 * @date    Feb 2016
 */

#ifndef __SWATCH_CORE_GATEKEEPERVIEW_HPP__
#define	__SWATCH_CORE_GATEKEEPERVIEW_HPP__

#include <map>
#include <set>
#include <string>

// boost headers
#include "boost/unordered_set.hpp"

// swatch headers
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/GenericView.hpp"

namespace swatch {
namespace core {

class GateKeeperView {
public:

  GateKeeperView(const GateKeeper& aGateKeeper);
  GateKeeperView(const GateKeeper& aGateKeeper, const std::vector<std::string>& aContextsToLookIn);
  ~GateKeeperView();

  typedef GenericView<GateKeeper::Parameter_t> ParameterView_t;
  typedef GenericView<GateKeeper::MonitoringSetting_t> MonitoringView_t;
  typedef GenericView<std::string> MaskView_t;
  typedef GenericView<std::string> DisabledFlagView_t;

  typedef boost::unordered_set<ParameterView_t> Parameters_t;
  typedef boost::unordered_set<MonitoringView_t> MonitoringSettings_t;
  typedef boost::unordered_set<MaskView_t> Masks_t;
  typedef boost::unordered_set<DisabledFlagView_t> DisabledFlags_t;

  Parameters_t::const_iterator parametersBegin() const;
  Parameters_t::const_iterator parametersEnd() const;
  const size_t parametersSize() const;

  MonitoringSettings_t::const_iterator monitoringSettingsBegin() const;
  MonitoringSettings_t::const_iterator monitoringSettingsEnd() const;
  const size_t monitoringSettingsSize() const;

  Masks_t::const_iterator masksBegin() const;
  Masks_t::const_iterator masksEnd() const;
  const size_t masksSize() const;

  DisabledFlags_t::const_iterator disabledFlagsBegin() const;
  DisabledFlags_t::const_iterator disabledFlagsEnd() const;
  const size_t disabledFlagsSize() const;

private:
  Parameters_t mParameters;
  MonitoringSettings_t mMonsettings;
  Masks_t mMasks;
  DisabledFlags_t mDisabledFlags;

  void extractDisabled(const GateKeeper& aGateKeeper);

  void extractMasks(const GateKeeper& aGateKeeper,
      const std::vector<std::string>& aContextsToLookIn = std::vector<std::string>());
  void extractMasks(const GateKeeper::MasksContext_t& aMasksContext, const std::string& aContext);

  void extractMonitoringSettings(const GateKeeper& aGateKeeper, const std::vector<std::string>& aContextsToLookIn =
      std::vector<std::string>());
//  void extractMonitoringSettings(const GateKeeper::SettingsContext_t& aMonSettingContext, const std::string& aContext);

  void extractParameters(const GateKeeper& aGateKeeper,
      const std::vector<std::string>& aContextsToLookIn = std::vector<std::string>());

  template<typename T>
  const std::vector<std::string> extractMapKeys(const boost::unordered_map<std::string, T>& aMap) const;
};

} // namespace core
} // namespace swatch

#include "swatch/core/GateKeeperView.hxx"

#endif	/* __SWATCH_CORE_GATEKEEPERVIEW_HPP__ */

