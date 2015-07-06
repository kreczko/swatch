/*
 * SystemCreator.hpp
 *
 *  Created on: 1 Oct 2014
 *      Author: kreczko
 *
 */

#ifndef __SWATCH_SYSTEM_SYSTEMLOGGINGCREATOR_HPP__
#define __SWATCH_SYSTEM_SYSTEMLOGGINGCREATOR_HPP__

#include "swatch/system/SystemCreator.hpp"

namespace swatch {
namespace system {

    
/*
 *  @brief The creator class for swatch::system::System that logs the progress of each step. 
 *  N.B. It is meant to be a direct copy of the default Creator and used in tests only.
 *  The current required structure for the ParameterSet is defined in
 *  swatch/test/etc/testdb.json.
 */
class SystemLoggingCreator : public swatch::system::SystemCreator {
public:
//    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::XParameterSet& params);
    virtual swatch::core::Object* operator()(const swatch::core::AbstractStub& aStub );
};


} // namespace system
} // namespace swatch


#endif /* __SWATCH_SYSTEM_SYSTEMLOGGINGCREATOR_HPP__ */
