#version 450

layout(binding = 0) uniform StaticUBO{
    mat4 view;
    mat4 proj;
} sUbo;

layout(binding = 1) uniform DynamicUBO{
    mat4 model;
} dUbo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = sUbo.proj * sUbo.view * dUbo.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
