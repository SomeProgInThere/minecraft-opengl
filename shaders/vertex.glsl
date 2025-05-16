#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform vec2 texOffset;
uniform vec2 texScale;

void main() {
    gl_Position = vec4(inPos, 1.0);
    texCoord = texOffset + inTexCoord * texScale;
}
