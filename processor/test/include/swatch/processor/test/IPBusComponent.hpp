/**
 * @file    IPBusComponent.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUSCOMPONENT_HPP
#define SWATCH_PROCESSOR_TEST_IPBUSCOMPONENT_HPP

// Boost Headers
#include <boost/unordered_map.hpp>

// uHAL Headers
 #include "uhal/HwInterface.hpp"

// Typedefs
//typedef boost::unordered_map<std::string, uint32_t> RegisterMap;

namespace swatch {
namespace processor {
namespace test {

// Helper class
class IPBusComponent {
private:
    uhal::HwInterface* hw_;
protected:
    IPBusComponent( uhal::HwInterface* hw ) : hw_(hw) {};
    uhal::HwInterface* hw() const {
        return hw_;
    }
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_TEST_IPBUSCOMPONENT_HPP */
