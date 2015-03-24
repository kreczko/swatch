/**
 * @file    ActionHandler.hpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_ACTIONHANDLER_HPP__
#define __SWATCH_CORE_ACTIONHANDLER_HPP__

#include "swatch/core/Command.hpp"
#include <boost/container/map.hpp>

#include <string>
#include "Operation.hpp"

namespace swatch {
namespace core {


/**
 * @class ActionHandler
 * @brief Interface that provides support to commands and operations
 */
class ActionHandler {
public:
    virtual ~ActionHandler();
    
    template<typename C>
    void registerCommand(const std::string aName );

    template<typename C>
        void registerOperation(const std::string name );

    Command* getCommand( const std::string& aName );
    Operation* getOperation( const std::string& name );

    /**
     * List of command names stored.
     * @return vector of command names
     */
    std::set<std::string> getCommands() const;

    /**
   * List of oepration names stored.
   * @return vector of oepration names
   */
  std::set<std::string> getOperations() const;

protected:
    ActionHandler();
    
private:

    CommandMap commands_;
    OperationMap operations_;

};


} // namespace core
} // namespace swatch
#include "swatch/core/ActionHandler.hxx"

#endif /* __SWATCH_CORE_ACTIONHANDLER_HPP__ */
