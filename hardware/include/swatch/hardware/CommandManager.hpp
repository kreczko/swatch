

#ifndef __SWATCH_CORE_COMMANDMANAGER_HPP__
#define __SWATCH_CORE_COMMANDMANAGER_HPP__

class CommandManager {
public:
    CommandManager();
    ~CommandManager();
private:
    std::string cmd_;
    std::vector<ActionHandler*> v;
};
#endif