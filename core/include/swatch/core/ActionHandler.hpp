/**
 * @file    CommandManager.hpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_TEST_ACTIONHANDLER__
#define __SWATCH_CORE_TEST_ACTIONHANDLER__ 

#include "swatch/core/Command.hpp"

#include <boost/unordered_map.hpp>

namespace swatch {
namespace core {


/**
 * @class ActionHandler
 * @details Interface that provides support to commands and operations
 */
class ActionHandler {
public:
    virtual ~ActionHandler();
    
    template<typename C>
    void registerCommand(std::string aName ) {

        // Check if aCmd has already been registered
        if ( commands_.count(aName) ) {
            // Make it an exception, because the command already exists
            return;
        }
  
        commands_.emplace(aName, new C(this) ) ;
    }

    Command* getCommand( const std::string& aName );

    /**
     * List of parameters names stored.
     * @return vector of parameter names
     */
    std::set<std::string> getCommands() const;

protected:
    ActionHandler();
    
private:

    typedef boost::unordered_map<std::string, Command*> CommandMap;
    
    CommandMap commands_;

};            

} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_TEST_ACTIONHANDLER__ */
