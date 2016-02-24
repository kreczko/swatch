#ifndef __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__
#define __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__

// MP7 headers
#include "mp7/definitions.hpp"
#include "mp7/PathConfigurator.hpp"

// SWATCH headers
#include "swatch/mp7/CommandChannelSelector.hpp"

namespace swatch {
namespace mp7 {



// Templated translator from BufferCore to BufferKind
template<class Selector>
struct BufferTraits {
    const static ::mp7::BufferKind bufferKind;
};

/**
 * @class ConfigureBuffersCommand
 */
template<class Selector>
class ConfigureBuffersCommand : public swatch::core::Command {
public:

  ConfigureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~ConfigureBuffersCommand() {}

  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);

protected:
  static const std::map< std::string, ::mp7::TestPathConfigurator::Mode > mBufferModeMap;

  // static std::map< std::string, ::mp7::TestPathConfigurator::Mode > initBufferModeMap();
  
  Selector mBufferSelector;
};

typedef ConfigureBuffersCommand<RxBufferSelector> ConfigureRxBuffersCommand;
typedef ConfigureBuffersCommand<TxBufferSelector> ConfigureTxBuffersCommand;


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
template<class Selector>
class SaveBuffersToFileCommand : public swatch::core::Command {
public:
  
  SaveBuffersToFileCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  virtual ~SaveBuffersToFileCommand() {}
  
  virtual swatch::core::Command::State code(const ::swatch::core::XParameterSet& params);

protected:

  Selector mBufferSelector;
};

typedef SaveBuffersToFileCommand<RxBufferSelector> SaveRxBuffersToFileCommand;
typedef SaveBuffersToFileCommand<TxBufferSelector> SaveTxBuffersToFileCommand;


/**
 * Command template to configure buffers in latency mode.
 * @tparam H Channel helper class 
 */
template<class Selector>
class LatencyBuffersCommand : public swatch::core::Command {
public:
  /**
   */
  LatencyBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  
  /**
   * Destructor
   */
  virtual ~LatencyBuffersCommand() {}
  
  /**
   * 
   * @param aParams
   * @return 
   */
  virtual State code(const ::swatch::core::XParameterSet& aParams);

private:
  //! Buffers helper class
  Selector mBufferSelector;
};

typedef LatencyBuffersCommand<RxBufferSelector> LatencyRxBuffersCommand;
typedef LatencyBuffersCommand<TxBufferSelector> LatencyTxBuffersCommand;


/**
 * 
 */
template<class Selector>
class EasyLatencyCommand : public swatch::core::Command {
public:
  EasyLatencyCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual ~EasyLatencyCommand() {}
  virtual State code(const ::swatch::core::XParameterSet& params);

protected:

  uint32_t computeLatency( uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal, uint32_t aRxExtraFramess, uint32_t aTxExtraFrames);

  Selector mBufferSelector;
};

typedef EasyLatencyCommand<RxBufferSelector> EasyRxLatencyCommand;
typedef EasyLatencyCommand<TxBufferSelector> EasyTxLatencyCommand;


}
}

#endif /* __SWATCH_MP7_MP7BUFFERCOMMANDS_HPP__ */
