#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexcord;

layout( push_constant ) uniform constants
{
	vec2 pos;
	float zoom;
} PushConstants;

void main() {
    gl_Position = vec4(inPosition+PushConstants.pos, 0.0, 1.0);
    fragColor = inColor;
    fragTexcord = inTexCoord;
}