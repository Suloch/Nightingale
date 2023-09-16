#include "commandqueue.hpp"
#include "../nightingale/logger/logger.hpp"

void nge::level::CommandQueue::pushCommand(CommandTypeEnum type, std::string data){
    this->commands.push_back(Command(type, data));
}

nge::level::Command::Command(CommandTypeEnum type, std::string data){
    this->data = data;
    this->type = type;
}


nge::level::Command nge::level::CommandQueue::getCommand(){

    if(this->commands.size() < 1)
        return Command(NULL_COMMAND, "");

    Command c =  this->commands.front();
    this->commands.erase(this->commands.begin());

    return c;
}