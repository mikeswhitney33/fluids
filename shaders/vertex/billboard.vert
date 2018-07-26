#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aOffset;


out vec2 TexCoord;

uniform vec3 billboard_center;
uniform mat4 view;
uniform vec2 billboard_size;
uniform mat4 projection;


void main() {
    vec3 center = aPos + aOffset;
    vec3 cameraRightWorld = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUpWorld = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 vertPosWorld = billboard_center +
        cameraRightWorld * center.x * billboard_size.x +
        cameraUpWorld * center.y * billboard_size.y;

    // vec3 vertPosWorld = aPos * billboard_size.x;
    gl_Position = projection * view * vec4(vertPosWorld, 1);
    TexCoord = aTexCoord;
}
