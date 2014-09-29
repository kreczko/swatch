/* 
 * File:   DummyProcessor.hpp
 * Author: ale
 *
 * Created on July 27, 2014, 9:19 PM
 */

#ifndef __swatch_test_dummyprocessor_hpp__
#define	__swatch_test_dummyprocessor_hpp__

#include "swatch/processor/Processor.hpp"

namespace swatch {
namespace system {
namespace test {

class DummyProcessor : public processor::Processor {
public:
    DummyProcessor( const std::string& aId, const core::ParameterSet& params );
    virtual ~DummyProcessor();
    
    virtual uint32_t getSlot() const { return mSlot; }
    virtual const std::string& getCrateId() const { return mCrateId; }
protected:
    uint32_t mSlot;
    std::string mCrateId;
    
};

} // namespace test
} // namespace system
} // namespace swatch

#endif	/* __swatch_test_dummyprocessor_hpp__ */

