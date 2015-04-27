/* 
 * File:   SwatchCOmmand.hxx
 * Author: ale
 *
 * Created on August 1, 2014, 12:30 PM
 */

#ifndef __SWATCH_CORE_ACTIONHANDLER_HXX__
#define	__SWATCH_CORE_ACTIONHANDLER_HXX__
#include "swatch/core/ActionHandler.hpp"
namespace swatch {
namespace core {
template<typename C>
void ActionHandler::registerCommand(const std::string aName) {

  // Check if aCmd has already been registered
  if (commands_.count(aName)) {
    // Make it an exception, because the command already exists
    return;
  }

  commands_.emplace(aName, new C(this));


}

template<typename C>
void ActionHandler::registerOperation(const std::string name) {

  // Check if name has already been registered
  if (operations_.count(name)) {
    // Make it an exception, because the controller already exists
    return;
  }

  operations_.emplace(name, new C(this));
}

} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_ACTIONHANDLER_HXX__ */
