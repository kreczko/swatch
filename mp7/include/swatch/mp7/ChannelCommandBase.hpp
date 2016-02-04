/* 
 * File:   ChannelCommandBase.hpp
 * Author: ale
 *
 * Created on February 4, 2016, 11:57 AM
 */

#ifndef __SWATCH_MP7_CHANNELCOMMANDBASE_HPP__
#define	__SWATCH_MP7_CHANNELCOMMANDBASE_HPP__

#include "swatch/core/Command.hpp"
#include "swatch/mp7/ChannelDescriptors.hpp"

#include "mp7/ChannelsManager.hpp"


namespace swatch {
namespace mp7 {

// Forward Declaration
class MP7AbstractProcessor;

/**
 * @class ChannelCommandBase
 */
class ChannelCommandBase : public swatch::core::Command {
public:
  template<typename ResultType>
  ChannelCommandBase( const std::string& aId, swatch::core::ActionableObject& aActionable, const ResultType& aDefault ) :
  swatch::core::Command(aId, aActionable, aDefault)
  {  
  }

  virtual ~ChannelCommandBase();
private:

  friend class ChannelCommandCore;
};

/**
 * @class ChannelCommandCore
 */
class ChannelCommandCore
{
public:
    ChannelCommandCore( ChannelCommandBase& );
    ~ChannelCommandCore();
    
    virtual void addParameters();
    ::mp7::ChannelsManager getManager( const swatch::core::XParameterSet& aParams ) const;

    virtual const ChannelsMap_t& getChannelDescriptors() const = 0;
    virtual std::string getIdSelection( const swatch::core::XParameterSet& aParams ) const;
    virtual const Rule_t& getGroupFilter() const = 0;
    virtual const Rule_t& getMaskFilter( const swatch::core::XParameterSet& aParams ) const;
    
    static const std::string kIdSelection;
    
protected:
    template<typename T>
    void registerParameter(const std::string& aName, const T& aDefaultValue);

    ChannelCommandBase& mCommand;
    const MP7AbstractProcessor& mProcessor;
    
    static const Rule_t kAlwaysTrue;
};

template<typename T>
void ChannelCommandCore::registerParameter(const std::string& aName, const T& aDefaultValue) {
  mCommand.registerParameter(aName, aDefaultValue);
}

/**
 * @class RxCommandCore
 */
class RxCommandCore : public ChannelCommandCore {
public:
  RxCommandCore( ChannelCommandBase& aCommand, const Rule_t& aFilter );

  virtual ~RxCommandCore() {}

  virtual void addParameters();
  
  virtual const Rule_t& getGroupFilter() const;
  
  virtual const ChannelsMap_t& getChannelDescriptors() const;

  virtual const Rule_t& getMaskFilter(const swatch::core::XParameterSet& aParams) const;


  static const std::string kMaskSelection;
  static const std::string kApplyMasks;
  static const std::string kInvertMasks;
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
class TxCommandCore : public ChannelCommandCore {
public:
  TxCommandCore(ChannelCommandBase& aCommand, const Rule_t& aFilter);
  
  virtual ~TxCommandCore() {}

  virtual const Rule_t& getGroupFilter() const;

  virtual const ChannelsMap_t& getChannelDescriptors() const;

private:
  const Rule_t mTxGroupFilter;
};


/**
 * @class RxMGTCommandCore
 */
class RxMGTCommandCore : public RxCommandCore {
public:
  
  RxMGTCommandCore( ChannelCommandBase& aCommand );
  
  virtual ~RxMGTCommandCore() {}
};


/**
 * @class TxMGTCommandCore
 */
class TxMGTCommandCore : public TxCommandCore {
public:
  TxMGTCommandCore(ChannelCommandBase& aCommand);

  virtual ~TxMGTCommandCore() {}

};

/**
 * @class RxBufferCommandCore
 */
class RxBufferCommandCore : public RxCommandCore {
public:
  
  RxBufferCommandCore( ChannelCommandBase& aCommand );
  
  virtual ~RxBufferCommandCore() {}
  
};


/**
 * @class TxBufferCommandCore
 */
class TxBufferCommandCore : public TxCommandCore {
public:
  TxBufferCommandCore(ChannelCommandBase& aCommand);

  virtual ~TxBufferCommandCore() {}

};


} // namespace mp7
} // namespace swatch

#endif /* __SWATCH_MP7_CHANNELCOMMANDBASE_HPP */


