#version 450
struct TextureBufferObject {
    float scale;
    float ar;
    float x;
    float y;
    int flipX;
    int flipY;
};

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexcord;

layout(location = 2) flat in TextureBufferObject tbo;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    // outColor = vec4(fragTexScale[2], fragTexScale[2], fragTexScale[2], 1);
    vec2 offsetCord = vec2(tbo.flipX*fragTexcord[0]*tbo.scale*tbo.ar, tbo.flipY*fragTexcord[1]*tbo.scale) +vec2(tbo.x, tbo.y);
    
    outColor = texture(texSampler, offsetCord);
}