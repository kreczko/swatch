/* 
 * File:   DummyProcessor.hpp
 * Author: ale
 *
 * Created on July 27, 2014, 9:19 PM
 */

#ifndef __SWATCH_TEST_DUMMYPROCESSOR_HPP__
#define	__SWATCH_TEST_DUMMYPROCESSOR_HPP__

#include "swatch/processor/Processor.hpp"
#include "swatch/core/Port.hpp"

namespace swatch {
namespace system {
namespace test {

/**
 * @class DummyRxPort
 * @brief Dummy input port implementation
 */
class DummyRxPort : public core::InputPort {
public:
  DummyRxPort(const std::string& aId);

  virtual ~DummyRxPort();

  virtual bool isEnabled() const;

  virtual bool isLocked() const;

  virtual bool isAligned() const;
  
  
  virtual uint32_t getCRCErrors() const;

};

/**
 * @class DummyTxPort
 * @brief Dummy ouput port implementation
 */
class DummyTxPort : public core::OutputPort {
public:
  DummyTxPort ( const std::string& aId);
  virtual ~DummyTxPort ();

  virtual bool isEnabled() const;

  virtual bool isOperating() const;

};


/**
 * @class DummyProcessor
 * @brief Dummy processor implementation
 */
class DummyProcessor : public processor::Processor {
public:
    DummyProcessor( const std::string& aId, const core::XParameterSet& aPars );
    virtual ~DummyProcessor();
    
    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;
    
    virtual void reset(const std::string& mode);


protected:
    uint32_t slot_;
    std::string crate_;
    
};

} // namespace test
} // namespace system
} // namespace swatch

#endif	/* __SWATCH_TEST_DUMMYPROCESSOR_HPP__ */

