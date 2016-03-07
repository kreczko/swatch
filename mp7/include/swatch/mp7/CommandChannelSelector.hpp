/* 
 * File:   ChannelCommandBase.hpp
 * Author: ale
 *
 * Created on February 4, 2016, 11:57 AM
 */

#ifndef __SWATCH_MP7_CHANNELCOMMANDBASE_HPP__
#define	__SWATCH_MP7_CHANNELCOMMANDBASE_HPP__

#include "swatch/core/Command.hpp"
#include "swatch/mp7/channel/DescriptorSelector.hpp"

#include "mp7/ChannelsManager.hpp"


namespace swatch {
namespace mp7 {

// Forward Declaration
class MP7AbstractProcessor;

/**
 * @class ChannelCommandSelector
 */
class CommandChannelSelector
{
public:
    CommandChannelSelector( swatch::core::Command& );
    ~CommandChannelSelector();
    
    virtual void addParameters();
    ::mp7::ChannelsManager getManager( const swatch::core::XParameterSet& aParams ) const;
    ::mp7::MP7Controller& getDriver();

    virtual const channel::DescriptorMap_t& getDescriptors() const = 0;
    virtual std::string getIdSelection( const swatch::core::XParameterSet& aParams ) const;
    virtual const channel::Rule_t& getGroupFilter() const = 0;
    virtual const channel::Rule_t& getMaskFilter( const swatch::core::XParameterSet& aParams ) const;
    
    static const std::string kIdSelection;
    
protected:
    swatch::core::Command& mCommand;
    MP7AbstractProcessor& mProcessor;
    
    static const channel::Rule_t kAlwaysTrue;
};

/**
 * @class RxCommandSelector
 */
class RxChannelSelector : public CommandChannelSelector {
public:
  RxChannelSelector( swatch::core::Command& aCommand, const channel::Rule_t& aFilter );

  virtual ~RxChannelSelector() {}

  virtual void addParameters();
  
  virtual const channel::Rule_t& getGroupFilter() const;
  
  virtual const channel::DescriptorMap_t& getDescriptors() const;

  virtual const channel::Rule_t& getMaskFilter(const swatch::core::XParameterSet& aParams) const;

  //!
  static const std::string kMaskSelection;
  
  //!
  static const std::string kApplyMasks;
  
  //!
  static const std::string kInvertMasks;
  
  //!
  static const std::string kIgnoreMasks;
  
private:
  const channel::Rule_t mRxGroupFilter;
  const channel::Rule_t mApplyMaskFilter;
  const channel::Rule_t mInvertMaskFilter;

};

DEFINE_SWATCH_EXCEPTION(ApplyMaskOptionInvalid);

/**
 * @class TxCommandSelector
 */
class TxChannelSelector : public CommandChannelSelector {
public:
  TxChannelSelector(swatch::core::Command& aCommand, const channel::Rule_t& aFilter);
  
  virtual ~TxChannelSelector() {}

  virtual const channel::Rule_t& getGroupFilter() const;

  virtual const channel::DescriptorMap_t& getDescriptors() const;

private:
  const channel::Rule_t mTxGroupFilter;
};


/**
 * @class RxMGTCommandSelector
 */
class RxMGTSelector : public RxChannelSelector {
public:
  
  RxMGTSelector( swatch::core::Command& aCommand );
  
  virtual ~RxMGTSelector() {}
};


/**
 * @class TxMGTCommandSelector
 */
class TxMGTSelector : public TxChannelSelector {
public:
  TxMGTSelector(swatch::core::Command& aCommand);

  virtual ~TxMGTSelector() {}

};

/**
 * @class RxBufferCommandSelector
 */
class RxBufferSelector : public RxChannelSelector {
public:
  
  RxBufferSelector( swatch::core::Command& aCommand );
  
  virtual ~RxBufferSelector() {}
  
};


/**
 * @class TxBufferCommandSelector
 */
class TxBufferSelector : public TxChannelSelector {
public:
  TxBufferSelector(swatch::core::Command& aCommand);

  virtual ~TxBufferSelector() {}

};


} // namespace mp7
} // namespace swatch

#endif /* __SWATCH_MP7_CHANNELCOMMANDBASE_HPP */


