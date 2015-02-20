/**
 * @file    CommandManager.hpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_TEST_COMMANDMANAGER__
#define __SWATCH_CORE_TEST_COMMANDMANAGER__ 

#include "swatch/core/Command.hpp"

#include <boost/unordered_map.hpp>

namespace swatch {
namespace core {


/**
 * @class ActionHandler
 * @details Abstract interface 
 */
class ActionHandler {
public:
    virtual ~ActionHandler();
    
    void registerCommand(std::string aName, Command * aCmd  );

    Command* getCommand( const std::string& aName );

protected:
    ActionHandler();
    
private:

    typedef boost::unordered_map<std::string, Command*> CommandMap;
    
    CommandMap commands_;

};            

} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_TEST_COMMANDMANAGER__ */
