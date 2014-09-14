/* 
 * File:   SystemCreator.hpp
 * Author: ale
 *
 * Created on August 12, 2014, 5:11 PM
 */

#ifndef SYSTEMCREATOR_HPP
#define	SYSTEMCREATOR_HPP

#include "swatch/system/SystemFactory.hpp"

namespace swatch {
namespace system {
namespace test {

class SystemCreator : public swatch::system::SystemFactory::CreatorInterface {
public:
    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::ParameterSet& aPSet);
};


} // namespace test
} // namespace system
} // namespace swatch

#endif	/* SYSTEMCREATOR_HPP */

