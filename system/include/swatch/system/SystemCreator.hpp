/*
 * SystemCreator.hpp
 *
 *  Created on: 1 Oct 2014
 *      Author: kreczko
 *
 *  The default creator class for swatch::system::System.
 *
 *  The current required structure for the ParameterSet is defined in
 *  swatch/test/etc/testdb.json.
 *
 */

#ifndef SYSTEMCREATOR_HPP
#define SYSTEMCREATOR_HPP

#include "swatch/system/SystemFactory.hpp"

namespace swatch {
namespace system {

class SystemCreator : public swatch::system::SystemFactory::CreatorInterface {
public:
    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::ParameterSet& params);

protected:
    virtual swatch::system::System* createSystem(const std::string& aId, const swatch::core::ParameterSet& params);
    virtual void addProcessors(swatch::system::System* system, const swatch::core::ParameterSet& params);
    virtual void addServices(swatch::system::System* system, const swatch::core::ParameterSet& params);
    virtual void addCrates(swatch::system::System* system, const swatch::core::ParameterSet& params);
    virtual void addCrate(swatch::system::System* system, const swatch::core::ParameterSet& params);
};


} // namespace system
} // namespace swatch


#endif /* SYSTEMCREATOR_HPP_ */
