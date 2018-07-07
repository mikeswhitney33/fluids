#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    FragColor = 0.7f * texture(ourTexture, TexCoord) + 0.3f * vec4(ourColor, 1);
}
