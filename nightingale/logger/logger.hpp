
#include <fstream>

namespace nge{
    enum LogLevel{
        info,
        debug,
        warn,
        error
    };
    class Logger{
        
        public:
            static Logger& getInstance(){
                static Logger logger;
                return logger;
            }
        private:
            bool started;
            LogLevel level;
            bool cerr;
            std::ofstream file;

            Logger(){}

        public:

            void start(LogLevel level, const char * filename);
            void log(const char* log);
            void log(const char* log, LogLevel level);
            void stop();
    };
}