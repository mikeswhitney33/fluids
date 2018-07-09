#version 330 core
out vec4 FragColor;

in vec4 Position;

const vec4 GREEN = vec4(0.0, 0.5, 0.01, 1.0);
const vec4 WHITE = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
    //FragColor = vec4(1.0f, 0.5f, 0.1f, 1.0f);
    FragColor = mix(GREEN, WHITE, Position.y);
}
