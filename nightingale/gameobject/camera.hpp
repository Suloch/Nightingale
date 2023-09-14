#pragma once
#include <glm/glm.hpp>

namespace nge
{
    class Camera2D{
        private:

            float height;
            float width;

            float x;
            float y;
            float z;

            float tx;
            float ty;
            float tz;
            
            float perspective;
            const float yoffset=0.00001;
        public:

            Camera2D(float height, float width);
            ~Camera2D();

            glm::mat4 getView();
            glm::mat4 getProjection();

            void setPosition(float x, float y);

    };
} // namespace nge
