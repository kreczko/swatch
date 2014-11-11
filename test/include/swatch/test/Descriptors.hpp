/**
 * @file    Data.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    09/11/14
 */

/*

          {
            "PROCESSOR NAME": "MP-1",
            "PROCESSOR TYPE": "CALO2 PROCESSOR",
            "PROCESSOR CREATOR": "DummyProcessor",
            "HARDWARE TYPE": "MP7",
            "CRATE NAME": "S2G17-01",
            "CRATE SLOT": 1,
            "CRATE LOCATION": "MyLocation",
            "CRATE DESCRIPTION": "MyCrateDescription",
            "PORTS NAMES": ["Port#1", "Port#2", "Port#3", "Port#144"],
              "IP ADDRESS": "192.168.0.2", 
              "PORTS": [  
                   {"PORT NAME" : "port#1", "PORT NUMBER" : "33", "TYPE" : "TX"},
                   {"PORT NAME" : "port#2", "PORT NUMBER" : "12", "TYPE" : "TX"},
                   {"PORT NAME" : "port#144", "PORT NUMBER" : "99", "TYPE" : "RX"}
                ]
          },
*/

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>


namespace swatch {
namespace test {

/**
 * @class PortDescriptor
 * @brief Structure to hold data to build a Processor port.
 * 
 */
struct PortDescriptor {
    //! Name of the port
    std::string  name;

    //! Port number
    uint32_t number;
};


/**
 * @class ProcessorBag
 * @brief Struct to hold the data to construct a processor
 */

struct ProcessorDescriptor {
    //! Name of the Processor
    std::string name;

    //! Class to create the Processor object
    std::string creator;

    //! Uri to access the hardware resource
    std::string uri;

    //! Address table
    std::string addressTable;

    //! Id of the uTCA crate where the Processor is installed 
    std::string crateId;

    //! Slot where the board is installed 
    uint32_t slot;

    //! List of receiver port bags
    std::vector<PortDescriptor> rxPorts;

    //! List of transmitter port bags
    std::vector<PortDescriptor> txPorts;
};

/**
 * @class AMC13Bag
 * @brief Struct to hold the data to construct an AMC13
 */
struct AMC13Bag {

};


std::ostream& operator<<(std::ostream& os, const ProcessorDescriptor& pd ) {
    
    os << "ProcessorDescriptor[" << pd.name << "]:\n"
       << "   - Uses creator = " << pd.creator << "\n"
       << "   - Uri = " << pd.uri << "\n"
       << "   - Address Table = " << pd.addressTable << "\n"
       << "   - Crate ID = " << pd.crateId 
       << "   - Crate Slot = " << pd.slot 
       << std::flush;


    return os;

}


} // processor
} // namespace swatch