
#include "../nightingale/nightingale.hpp"
#include<string>
#include<thread>
namespace nge{
    class LevelBuilder{
        
        Nightingale *app;
        int height;
        int width;
        std::thread commandThread;
        std::string sceneName;
        
        void runCommands();
        void loadScene();
        void saveScene();

        public:
            LevelBuilder(std::string sceneName);
            LevelBuilder();
            ~LevelBuilder();
            
            void run();
    };
}