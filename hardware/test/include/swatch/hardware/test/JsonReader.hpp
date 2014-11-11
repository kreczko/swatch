/**
 * @file    JsonReader.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef SWATCH_HARDWARE_TEST_JSONREADER_HPP
#define SWATCH_HARDWARE_TEST_JSONREADER_HPP

#include "swatch/core/ParameterSet.hpp"

namespace swatch {
namespace hardware {
namespace test {

swatch::core::ParameterSet readJson( const std::string path );

}
}
}

#endif