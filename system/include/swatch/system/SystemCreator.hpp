/*
 * SystemCreator.hpp
 *
 *  Created on: 1 Oct 2014
 *      Author: kreczko
 *
 *  The default creator class for swatch::system::System.
 *
 *  The current required structure for the XParameterSet is defined in
 *  swatch/test/etc/testdb.json.
 *
 */

#ifndef __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__
#define __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__


#include "swatch/core/Factory.hpp"
#include "swatch/core/exception.hpp"


namespace swatch {
namespace system {

class System;

//class SystemCreator : public swatch::system::SystemFactory::CreatorInterface {
class SystemCreator : public swatch::core::Factory::CreatorInterface {
public:
//    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::XParameterSet& params);
    virtual swatch::core::Object* operator()(const std::string& aId, const swatch::core::XParameterSet& params);

protected:
    virtual swatch::system::System* createSystem(const std::string& aId, const swatch::core::XParameterSet& params);
    virtual void addProcessors(swatch::system::System* system, const swatch::core::XParameterSet& params);
    virtual void addDaqTTCs(swatch::system::System* system, const swatch::core::XParameterSet& params);
    virtual void addServices(swatch::system::System* system, const swatch::core::XParameterSet& params);
    virtual void addCrates(swatch::system::System* system, const swatch::core::XParameterSet& params);
    virtual void addCrate(swatch::system::System* system, const swatch::core::XParameterSet& params);
    virtual void addLinks(swatch::system::System* system, const swatch::core::XParameterSet& params);    
};

DEFINE_SWATCH_EXCEPTION(SystemCreationFailed);

} // namespace system
} // namespace swatch


#endif /* __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__ */
