#version 460 core

in vec2 texCoord;
in float shading;

uniform sampler2D sampledTexture;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(sampledTexture, texCoord);
    fragColor = vec4(texColor.rgb * shading, texColor.a);
}

