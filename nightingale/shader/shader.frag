#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexcord;
layout(location = 2) in vec3  fragTexScale;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

void main() {
    //outColor = vec4(texScale, texScale, texScale, 1);
    vec2 offsetCord = vec2(fragTexcord*fragTexScale[0])+vec2(fragTexScale[1], fragTexScale[2]);
    outColor = texture(texSampler, offsetCord);
}