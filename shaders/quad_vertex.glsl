#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in uint inFaceIndex;

out vec2 texCoord;
out float shading;

uniform mat4 cameraView;
uniform mat4 cameraProj;

const float normalShade[6] = float[6](
    1.0, 0.4, // up, down
    0.4, 0.7, // right, left
    0.4, 0.7 // front, back
);

void main() {
    gl_Position = cameraProj * cameraView * mat4(1.0) * vec4(inPosition, 1.0);
    texCoord = inTexCoord;
    shading = normalShade[0];
}