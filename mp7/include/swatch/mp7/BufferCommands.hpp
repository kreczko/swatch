#ifndef __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__
#define __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__

// MP7 headers
#include "mp7/definitions.hpp"

// SWATCH headers
#include "swatch/mp7/AbstractChannelsCommand.hpp"


namespace swatch {
namespace mp7 {


// Templated translator from ChannelGroup to BufferKind
template<AbstractChannelsCommand::ChannelGroup Group>
struct BufferTraits {
    const static ::mp7::BufferKind bufferKind;
};


/**
 * @class ConfigureBuffersCommand
 */
template<AbstractChannelsCommand::ChannelGroup tGroup>
class ConfigureBuffersCommand : public AbstractChannelsCommand {
public:

  ConfigureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~ConfigureBuffersCommand();

  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);

private:
  static const std::map< std::string, ::mp7::TestPathConfigurator::Mode > mBufferModeMap;

  static std::map< std::string, ::mp7::TestPathConfigurator::Mode > initBufferModeMap();

};

typedef ConfigureBuffersCommand<AbstractChannelsCommand::kRx> ConfigureRxBuffersCommand;
typedef ConfigureBuffersCommand<AbstractChannelsCommand::kTx> ConfigureTxBuffersCommand;


/**
 * @class CaptureBuffersCommand
 */
class CaptureBuffersCommand : public ::swatch::core::Command {
public:
  CaptureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~CaptureBuffersCommand();
  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);
};


/**
 * @class SaveBuffersToFileCommand
 */
template<AbstractChannelsCommand::ChannelGroup tGroup>
class SaveBuffersToFileCommand : public AbstractChannelsCommand {
public:
  SaveBuffersToFileCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~SaveBuffersToFileCommand();
  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);
};

typedef SaveBuffersToFileCommand<AbstractChannelsCommand::kRx> SaveRxBuffersToFileCommand;
typedef SaveBuffersToFileCommand<AbstractChannelsCommand::kTx> SaveTxBuffersToFileCommand;


/**
 * @class LatencyBuffersCommand
 */
template<AbstractChannelsCommand::ChannelGroup tGroup>
class LatencyBuffersCommand : public AbstractChannelsCommand {
public:
  LatencyBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~LatencyBuffersCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);
private:

};

typedef LatencyBuffersCommand<AbstractChannelsCommand::kRx> LatencyRxBuffersCommand;
typedef LatencyBuffersCommand<AbstractChannelsCommand::kTx> LatencyTxBuffersCommand;


/**
 * @class EasyLatencyCommand
 */
template<AbstractChannelsCommand::ChannelGroup tGroup>
class EasyLatencyCommand : public AbstractChannelsCommand {
public:
  EasyLatencyCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~EasyLatencyCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);
private:
  static uint32_t computeLatency( uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal);
};

typedef EasyLatencyCommand<AbstractChannelsCommand::kRx> EasyRxLatencyCommand;
typedef EasyLatencyCommand<AbstractChannelsCommand::kTx> EasyTxLatencyCommand;


}
}

#endif /* __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__ */
