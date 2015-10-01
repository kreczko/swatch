/*
 * File:   SystemCreator.hpp
 * Author: L. Kreczko
 * Date:   Oct 2014
 */

#ifndef __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__
#define __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__


#include "swatch/core/exception.hpp"
#include "swatch/core/Factory.hpp"


namespace swatch {
namespace processor {

class LinkStub;

}
namespace system {

class System;
class CrateStub;

/*
//! The default creator class for swatch::system::System
class SystemCreator : public swatch::core::Factory::CreatorInterface {
public:
    virtual swatch::core::Object* operator()(const swatch::core::AbstractStub& aStub);

protected:
    virtual swatch::system::System* createSystem(const swatch::core::AbstractStub& aStub);
    virtual void addProcessors(swatch::system::System* system);
    virtual void addDaqTTCs(swatch::system::System* system);
//    virtual void addServices(swatch::system::System* system);
    virtual void addCrates(swatch::system::System* system);
    virtual void addCrate(swatch::system::System* system, const swatch::system::CrateStub& aStub );
    virtual void addLinks(swatch::system::System* system );    
};
*/

} // namespace system
} // namespace swatch


#endif /* __SWATCH_SYSTEM_SYSTEMCREATOR_HPP__ */
