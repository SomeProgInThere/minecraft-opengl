#version 460 core

in vec2 texCoord;
flat in float shading;

uniform sampler2D sampledTexture;

out vec4 fragColor;

void main() {
    fragColor = texture(sampledTexture, texCoord) * shading;
}
