#version 450

layout(location = 0) out vec3 fragColor;
layout( push_constant ) uniform constants
{
	vec2 pos;
	float zoom;
} PushConstants;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(0.0, 0.0, 1.0),
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    vec2 position = positions[gl_VertexIndex] + PushConstants.pos;
    gl_Position = vec4(position, 0.0, PushConstants.zoom);
    fragColor = colors[gl_VertexIndex];
}