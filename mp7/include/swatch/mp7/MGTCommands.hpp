#ifndef __SWATCH_MP7_MP7MGTCOMMANDS_HPP__
#define __SWATCH_MP7_MP7MGTCOMMANDS_HPP__


#include "swatch/mp7/AbstractChannelsCommand.hpp"


namespace swatch {
namespace mp7 {

class ConfigureRxMGTsCommand : public AbstractChannelsCommand {
public:
    ConfigureRxMGTsCommand(const std::string& aId);
    virtual ~ConfigureRxMGTsCommand();
    virtual core::Command::State code(const core::XParameterSet& params);

};


class ConfigureTxMGTsCommand : public AbstractChannelsCommand {
public:
    ConfigureTxMGTsCommand(const std::string& aId);
    virtual ~ConfigureTxMGTsCommand();
    virtual core::Command::State code(const core::XParameterSet& params);

};


class AlignRxsToCommand : public AbstractChannelsCommand {
public:
    AlignRxsToCommand(const std::string& aId);
    virtual ~AlignRxsToCommand();
    virtual core::Command::State code(const core::XParameterSet& params);

};


class AutoAlignCommand : public AbstractChannelsCommand {
public:
    AutoAlignCommand(const std::string& aId);
    virtual ~AutoAlignCommand();
    virtual core::Command::State code (const core::XParameterSet& aParams);
};


}
}

#endif /* __SWATCH_MP7_MP7MGTCOMMANDS_HPP__ */
