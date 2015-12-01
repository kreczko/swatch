/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IdSliceParser.hpp
 * Author: ale
 *
 * Created on November 16, 2015, 4:58 PM
 */

#ifndef __SWATCH_CORE_TOOLBOX_IDSLICEPARSER_HPP__
#define __SWATCH_CORE_TOOLBOX_IDSLICEPARSER_HPP__

#include <vector>
#include <string>
 
namespace swatch {
namespace core {
namespace toolbox {
    
struct IdSliceParser {
    static std::vector<std::string> parse( const std::string& aStringSlice );
};

} // namespace toolbox 
} // namespace core 
} // namespace swatch

#endif /* __SWATCH_CORE_TOOLBOX_IDSLICEPARSER_HPP__ */
