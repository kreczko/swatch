#ifndef __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__
#define __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__

// MP7 headers
#include "mp7/definitions.hpp"
#include "mp7/PathConfigurator.hpp"

// SWATCH headers
#include "swatch/mp7/ChannelCommandBase.hpp"

namespace swatch {
namespace mp7 {



// Templated translator from BufferCore to BufferKind
template<class C>
struct BufferTraits {
    const static ::mp7::BufferKind bufferKind;
};

/**
 * @class ConfigureBuffersCommand
 */
template<class C>
class ConfigureBuffersCommand : public ChannelCommandBase {
public:

  ConfigureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~ConfigureBuffersCommand() {}

  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);

private:
  static const std::map< std::string, ::mp7::TestPathConfigurator::Mode > mBufferModeMap;

  static std::map< std::string, ::mp7::TestPathConfigurator::Mode > initBufferModeMap();
  
  C mCore;
};

typedef ConfigureBuffersCommand<RxBufferCommandCore> ConfigureRxBuffersCommand;
typedef ConfigureBuffersCommand<TxBufferCommandCore> ConfigureTxBuffersCommand;


/**
 * @class CaptureBuffersCommand
 */
class CaptureBuffersCommand : public ::swatch::core::Command {
public:
  CaptureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~CaptureBuffersCommand() {}
  
  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);
};


/**
 * @class SaveBuffersToFileCommand
 */
template<class C>
class SaveBuffersToFileCommand : public ChannelCommandBase {
public:
  
  SaveBuffersToFileCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~SaveBuffersToFileCommand() {}
  
  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);

private:

  C mCore;
};

typedef SaveBuffersToFileCommand<RxBufferCommandCore> SaveRxBuffersToFileCommand;
typedef SaveBuffersToFileCommand<TxBufferCommandCore> SaveTxBuffersToFileCommand;


/**
 * @class LatencyBuffersCommand
 */
template<class C>
class LatencyBuffersCommand : public ChannelCommandBase {
public:
  LatencyBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~LatencyBuffersCommand() {}
  virtual State code(const ::swatch::core::XParameterSet& params);
private:
  
  C mCore;
};

typedef LatencyBuffersCommand<RxBufferCommandCore> LatencyRxBuffersCommand;
typedef LatencyBuffersCommand<TxBufferCommandCore> LatencyTxBuffersCommand;


/**
 * @class EasyLatencyCommand
 */
template<class C>
class EasyLatencyCommand : public ChannelCommandBase {
public:
  EasyLatencyCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~EasyLatencyCommand() {}
  virtual State code(const ::swatch::core::XParameterSet& params);
private:

  static uint32_t computeLatency( uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal);

  C mCore;
};

typedef EasyLatencyCommand<RxBufferCommandCore> EasyRxLatencyCommand;
typedef EasyLatencyCommand<TxBufferCommandCore> EasyTxLatencyCommand;


}
}

#endif /* __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__ */
