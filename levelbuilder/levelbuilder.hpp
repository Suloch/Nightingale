
#include "../nightingale/nightingale.hpp"
#include<string>

namespace nge{
    class LevelBuilder{
        
        Nightingale *app;
        int height;
        int width;

        std::string sceneName;

        void loadScene();
        void saveScene();

        public:
            LevelBuilder(std::string sceneName);
            LevelBuilder();
            ~LevelBuilder();
            
            void run();
    };
}