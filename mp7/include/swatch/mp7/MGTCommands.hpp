#ifndef __SWATCH_MP7_MP7MGTCOMMANDS_HPP__
#define __SWATCH_MP7_MP7MGTCOMMANDS_HPP__


#include "swatch/mp7/ChannelCommandCore.hpp"


namespace swatch {
namespace mp7 {

class ConfigureRxMGTsCommand : public swatch::core::Command {
public:
  ConfigureRxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~ConfigureRxMGTsCommand() {}
  virtual core::Command::State code(const core::XParameterSet& params);

private:
  RxMGTCommandCore mRxMGTCore;
};


class ConfigureTxMGTsCommand : public swatch::core::Command {
public:
    ConfigureTxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~ConfigureTxMGTsCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  TxMGTCommandCore mTxMGTCore;
};


class AlignRxsToCommand : public swatch::core::Command {
public:
    AlignRxsToCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AlignRxsToCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  RxMGTCommandCore mRxMGTCore;
};


class AutoAlignCommand : public swatch::core::Command {
public:
    AutoAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AutoAlignCommand() {}
    virtual core::Command::State code (const core::XParameterSet& aParams);
private:
  RxMGTCommandCore mRxMGTCore;
};


}
}

#endif /* __SWATCH_MP7_MP7MGTCOMMANDS_HPP__ */
