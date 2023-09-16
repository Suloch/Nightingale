
#pragma once

#include<vector>
#include<string>

namespace nge
{
    namespace level{
        enum CommandTypeEnum{
            NULL_COMMAND,
            CREATE_TEXTURE,
            STOP_EXECUTE
        };

        class Command{
            public:
                CommandTypeEnum type;
                std::string data;

                Command(CommandTypeEnum type, std::string data);
        };

        class CommandQueue{
            public:
                static CommandQueue& getInstance(){
                    static CommandQueue commandQueue;
                    return commandQueue;
                }
                void pushCommand(CommandTypeEnum type, std::string command);
                Command getCommand();
            private:
                std::vector<Command> commands;

        };
    }
} // namespace nge
