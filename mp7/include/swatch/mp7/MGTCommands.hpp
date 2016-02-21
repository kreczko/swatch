#ifndef __SWATCH_MP7_MP7MGTCOMMANDS_HPP__
#define __SWATCH_MP7_MP7MGTCOMMANDS_HPP__


#include "swatch/mp7/CommandChannelSelector.hpp"


namespace swatch {
namespace mp7 {

class ConfigureRxMGTsCommand : public swatch::core::Command {
public:
  ConfigureRxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~ConfigureRxMGTsCommand() {}
  virtual core::Command::State code(const core::XParameterSet& params);

private:
  RxMGTSelector mRxMGTHelper;
};


class ConfigureTxMGTsCommand : public swatch::core::Command {
public:
    ConfigureTxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~ConfigureTxMGTsCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  TxMGTSelector mTxMGTHelper;
};


class AlignRxsToCommand : public swatch::core::Command {
public:
    AlignRxsToCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AlignRxsToCommand() {}
    virtual core::Command::State code(const core::XParameterSet& params);
private:
  RxMGTSelector mRxMGTHelper;
};


class AutoAlignCommand : public swatch::core::Command {
public:
    AutoAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
    virtual ~AutoAlignCommand() {}
    virtual core::Command::State code (const core::XParameterSet& aParams);
private:
  RxMGTSelector mRxMGTHelper;
};


}
}

#endif /* __SWATCH_MP7_MP7MGTCOMMANDS_HPP__ */
