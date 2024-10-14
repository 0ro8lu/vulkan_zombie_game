#version 450

layout(binding = 0) uniform StaticUBO{
    mat4 view;
    mat4 proj;
} sUbo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 2) in vec3 position;
layout(location = 3) in vec3 rotation;
layout(location = 4) in float scale;
layout(location = 5) in int textureID;

layout(location = 0) out vec3 fragTexCoord;

// Function to create a rotation matrix around the X axis
mat4 rotateX(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, c, -s, 0.0,
        0.0, s, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// Function to create a rotation matrix around the Y axis
mat4 rotateY(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c, 0.0, s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

// Function to create a rotation matrix around the Z axis
mat4 rotateZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c, -s, 0.0, 0.0,
        s, c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main() {
    // Create scaling matrix
    mat4 scaleMat = mat4(
        scale, 0.0, 0.0, 0.0,
        0.0, scale, 0.0, 0.0,
        0.0, 0.0, scale, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Create rotation matrix
    mat4 rotMat = rotateZ(rotation.z) * rotateY(rotation.y) * rotateX(rotation.x);

    // Create translation matrix
    mat4 transMat = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        position.x, position.y, position.z, 1.0
    );

    // Combine matrices to create model matrix
    mat4 modelMat = transMat * rotMat * scaleMat;

    // Apply model matrix to vertex position
    vec4 worldPos = modelMat * vec4(inPosition, 0.0, 1.0);

    // Calculate final position
    gl_Position = sUbo.proj * sUbo.view * worldPos;

    fragTexCoord = vec3(inTexCoord, textureID);
}
