#version 460 core

#define MAX_BLOCK_COUNT 1

struct FaceTexCoords {
    vec2 topLeft;
    vec2 bottomRight;
};

layout (std140, binding = 1) uniform BlockTextureData {
    FaceTexCoords texCoords[MAX_BLOCK_COUNT * 6];
} blockTextures;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in uint inNormalIndex;
layout (location = 2) in uint inBlockIndex;

out vec2 texCoord;
flat out float shading;

uniform mat4 cameraView;
uniform mat4 cameraProj;

const float normalShade[6] = float[6](
    1.0, 0.4, // up, down
    0.4, 0.7, // right, left
    0.4, 0.7 // front, back
);

void main() {
    gl_Position = cameraProj * cameraView * mat4(1.0) * vec4(inPosition, 1.0);

    uint textureIndex = inBlockIndex * 6 + inNormalIndex;
    FaceTexCoords faceCoord = blockTextures.texCoords[textureIndex];
    texCoord = faceCoord.topLeft + vec2(1.0, 1.0) * (faceCoord.bottomRight - faceCoord.topLeft);

    shading = normalShade[inNormalIndex];
}