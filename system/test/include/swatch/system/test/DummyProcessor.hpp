/* 
 * File:   DummyProcessor.hpp
 * Author: ale
 *
 * Created on July 27, 2014, 9:19 PM
 */

#ifndef __SWATCH_TEST_DUMMYPROCESSOR_HPP__
#define	__SWATCH_TEST_DUMMYPROCESSOR_HPP__

#include "swatch/processor/Processor.hpp"

namespace swatch {
namespace system {
namespace test {

class DummyProcessor : public processor::Processor {
public:
    DummyProcessor( const std::string& aId, const core::XParameterSet& aPars );
    virtual ~DummyProcessor();
    
    virtual uint32_t getSlot() const { return slot_; }
    virtual const std::string& getCrateId() const { return crate_; }
    

    virtual std::set<std::string> getModes() const;

    virtual void reset(const std::string& mode);


protected:
    uint32_t slot_;
    std::string crate_;
    
};

} // namespace test
} // namespace system
} // namespace swatch

#endif	/* __SWATCH_TEST_DUMMYPROCESSOR_HPP__ */

