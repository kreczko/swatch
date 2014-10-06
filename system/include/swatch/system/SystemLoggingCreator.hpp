/*
 * SystemCreator.hpp
 *
 *  Created on: 1 Oct 2014
 *      Author: kreczko
 *
 *  The creator class for swatch::system::System that logs the progress of each
 *  step. It is meant to be a direct copy of the default Creator and used
 *  in tests only.
 *  The current required structure for the ParameterSet is defined in
 *  swatch/test/etc/testdb.json.
 *
 */

#ifndef SYSTEMLOGGINGCREATOR_HPP
#define SYSTEMLOGGINGCREATOR_HPP

#include "swatch/system/SystemCreator.hpp"

namespace swatch {
namespace system {

class SystemLoggingCreator : public swatch::system::SystemCreator {
public:
    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::ParameterSet& params);
};


} // namespace system
} // namespace swatch


#endif /* SYSTEMCREATOR_HPP_ */
