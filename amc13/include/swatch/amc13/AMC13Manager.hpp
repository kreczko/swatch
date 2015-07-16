/**
 * @file    AMC13Manager.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#ifndef SWATCH_AMC13_AMC13MANAGER_HPP
#define SWATCH_AMC13_AMC13MANAGER_HPP


// Swatch Headers
#include "swatch/system/DaqTTCManager.hpp"


// Forward declaration
namespace amc13 {
class AMC13;
}

namespace swatch {
namespace amc13 {

class AMC13Manager : public swatch::system::DaqTTCManager {
public:
    AMC13Manager( const swatch::core::AbstractStub& aStub );
    ~AMC13Manager();
    
    virtual void reset();
    
    virtual void configureClock(const std::string& mode);

    virtual void enableTTC(const std::vector<uint32_t>& aSlots);

    ::amc13::AMC13& driver() { return *driver_; }

    /*
    virtual double ttcClockFreq() const;
    
    virtual uint32_t ttcBC0Counter() const;
    
    virtual uint32_t ttcBC0Errors() const;
    
    virtual uint32_t ttcSingleBitErrors() const;
    
    virtual uint32_t ttcDoubleBitErrors() const;
    */

protected:
    virtual void retrieveMetricValues();
    
private:
    ::amc13::AMC13* driver_;
};

} // namespace amc13
} // namespace swatch

#endif /* SWATCH_AMC13_AMC13MANAGER_HPP */
