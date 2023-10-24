#pragma once


namespace nge{
    class Transform2D{
        public:
            float x;
            float y;
            float z;
            float scaleX;
            float scaleY;
            float rot;

            Transform2D(int height, int width){
                x = 0;
                y = 0;
                z = 0;
                scaleX = 1;
                scaleY = 1;
                rot = 0;
                screenHeight = height;
                screenWidth = width;
            }
            
            float getX(){return x/screenWidth;}
            float getY(){return y/screenHeight;}
            float getZ(){return z/screenHeight;}

        private:
            int screenHeight;
            int screenWidth;
        

    };
}