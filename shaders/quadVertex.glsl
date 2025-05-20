#version 460 core

layout (location = 0) in vec3 quadPos;
layout (location = 1) in vec2 quadTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 texOffset;
uniform vec2 texScale;

void main() {
    gl_Position = projection * view * model * vec4(quadPos, 1.0);
    texCoord = texOffset + quadTexCoord * texScale;
}
