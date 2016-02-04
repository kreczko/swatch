#ifndef __SWATCH_MP7_MP7MGTCOMMANDS_HPP__
#define __SWATCH_MP7_MP7MGTCOMMANDS_HPP__


#include "swatch/mp7/ChannelCommandBase.hpp"


namespace swatch {
namespace mp7 {

class ConfigureRxMGTsCommand : public ChannelCommandBase {
public:
  ConfigureRxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~ConfigureRxMGTsCommand() {}
  virtual core::Command::State code(const core::XParameterSet& params);

private:
  RxMGTCommandCore mCore;
};


class ConfigureTxMGTsCommand : public ChannelCommandBase {
public:
    ConfigureTxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~ConfigureTxMGTsCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  TxMGTCommandCore mCore;
};


class AlignRxsToCommand : public ChannelCommandBase {
public:
    AlignRxsToCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AlignRxsToCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  RxMGTCommandCore mCore;
};


class AutoAlignCommand : public ChannelCommandBase {
public:
    AutoAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AutoAlignCommand() {}
    virtual core::Command::State code (const core::XParameterSet& aParams);
private:
  RxMGTCommandCore mCore;
};


}
}

#endif /* __SWATCH_MP7_MP7MGTCOMMANDS_HPP__ */
