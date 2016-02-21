/* 
 * File:   ChannelCommandBase.hpp
 * Author: ale
 *
 * Created on February 4, 2016, 11:57 AM
 */

#ifndef __SWATCH_MP7_CHANNELCOMMANDBASE_HPP__
#define	__SWATCH_MP7_CHANNELCOMMANDBASE_HPP__

#include "swatch/core/Command.hpp"
#include "swatch/mp7/ChannelDescriptor.hpp"

#include "mp7/ChannelsManager.hpp"


namespace swatch {
namespace mp7 {

// Forward Declaration
class MP7AbstractProcessor;

/**
 * @class ChannelCommandCore
 */
class CommandChannelSelector
{
public:
    CommandChannelSelector( swatch::core::Command& );
    ~CommandChannelSelector();
    
    virtual void addParameters();
    ::mp7::ChannelsManager getManager( const swatch::core::XParameterSet& aParams ) const;
    ::mp7::MP7Controller& getDriver();

    virtual const ChannelsMap_t& getChannelDescriptors() const = 0;
    virtual std::string getIdSelection( const swatch::core::XParameterSet& aParams ) const;
    virtual const Rule_t& getGroupFilter() const = 0;
    virtual const Rule_t& getMaskFilter( const swatch::core::XParameterSet& aParams ) const;
    
    static const std::string kIdSelection;
    
protected:
    swatch::core::Command& mCommand;
    MP7AbstractProcessor& mProcessor;
    
    static const Rule_t kAlwaysTrue;
};

/**
 * @class RxCommandCore
 */
class RxChannelSelector : public CommandChannelSelector {
public:
  RxChannelSelector( swatch::core::Command& aCommand, const Rule_t& aFilter );

  virtual ~RxChannelSelector() {}

  virtual void addParameters();
  
  virtual const Rule_t& getGroupFilter() const;
  
  virtual const ChannelsMap_t& getChannelDescriptors() const;

  virtual const Rule_t& getMaskFilter(const swatch::core::XParameterSet& aParams) const;

  //!
  static const std::string kMaskSelection;
  
  //!
  static const std::string kApplyMasks;
  
  //!
  static const std::string kInvertMasks;
  
  //!
  static const std::string kIgnoreMasks;
  
private:
  const Rule_t mRxGroupFilter;
  const Rule_t mApplyMaskFilter;
  const Rule_t mInvertMaskFilter;

};

DEFINE_SWATCH_EXCEPTION(ApplyMaskOptionInvalid);

/**
 * @class TxCommandCore
 */
class TxChannelSelector : public CommandChannelSelector {
public:
  TxChannelSelector(swatch::core::Command& aCommand, const Rule_t& aFilter);
  
  virtual ~TxChannelSelector() {}

  virtual const Rule_t& getGroupFilter() const;

  virtual const ChannelsMap_t& getChannelDescriptors() const;

private:
  const Rule_t mTxGroupFilter;
};


/**
 * @class RxMGTCommandCore
 */
class RxMGTSelector : public RxChannelSelector {
public:
  
  RxMGTSelector( swatch::core::Command& aCommand );
  
  virtual ~RxMGTSelector() {}
};


/**
 * @class TxMGTCommandCore
 */
class TxMGTSelector : public TxChannelSelector {
public:
  TxMGTSelector(swatch::core::Command& aCommand);

  virtual ~TxMGTSelector() {}

};

/**
 * @class RxBufferCommandCore
 */
class RxBufferSelector : public RxChannelSelector {
public:
  
  RxBufferSelector( swatch::core::Command& aCommand );
  
  virtual ~RxBufferSelector() {}
  
};


/**
 * @class TxBufferCommandCore
 */
class TxBufferSelector : public TxChannelSelector {
public:
  TxBufferSelector(swatch::core::Command& aCommand);

  virtual ~TxBufferSelector() {}

};


} // namespace mp7
} // namespace swatch

#endif /* __SWATCH_MP7_CHANNELCOMMANDBASE_HPP */


