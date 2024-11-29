#version 450

layout(binding = 0) uniform StaticUBO{
    mat4 view;
    mat4 proj;
} sUbo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 2) in mat4 model;
layout(location = 6) in int textureID;

layout(location = 0) out vec3 fragTexCoord;

void main() {

    // Apply model matrix to vertex position
    vec4 worldPos = model * vec4(inPosition, 0.0, 1.0);

    // Calculate final position
    gl_Position = sUbo.proj * sUbo.view * worldPos;

    fragTexCoord = vec3(inTexCoord, textureID);
}
