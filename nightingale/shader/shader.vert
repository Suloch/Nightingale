#version 450

layout( push_constant ) uniform constants
{
	vec2 pos;
	float zoom;
} PushConstants;


struct TextureBufferObject{
    float scale;
    float ar;
    float x;
    float y;
    int flipX;
    int flipY;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    TextureBufferObject tex;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out TextureBufferObject tbo;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    fragTexCoord = inTexCoord;
    fragColor = inColor;
    tbo = ubo.tex;
}