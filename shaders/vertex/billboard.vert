#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


out vec2 TexCoord;

uniform vec3 billboard_center;
uniform mat4 view;
uniform vec2 billboard_size;


void main() {
    vec3 cameraRightWorld = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUpWorld = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 vertPosWorld = billboard_center +
        cameraRightWorld * aPos.x * billboard_size.x +
        cameraUpWorld * aPos.y * billboard_size.y;
    gl_Position = vec4(vertPosWorld, 1);
    TexCoord = aTexCoord;
}
