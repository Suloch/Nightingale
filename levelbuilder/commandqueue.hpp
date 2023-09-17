
#pragma once

#include<vector>
#include<string>

namespace nge
{
    namespace level{
        enum CommandTypeEnum{
            NULL_COMMAND,
            CREATE_TEXTURE,
            CREATE_OBJECT,
            STOP_EXECUTE
        };

        class Command{
            public:
                CommandTypeEnum type;
                std::vector<std::string> data;

                Command(CommandTypeEnum type, std::vector<std::string> data);
        };

        class CommandQueue{
            public:
                static CommandQueue& getInstance(){
                    static CommandQueue commandQueue;
                    return commandQueue;
                }
                void pushCommand(CommandTypeEnum type, std::vector<std::string> command);
                Command getCommand();
            private:
                std::vector<Command> commands;

        };
    }
} // namespace nge
