#version 460 core
layout (location = 0) in vec3 Position;
out vec4 VertexColor;
void main() {
    gl_position = vec4(Position, 1.0);
    VertexColor = vec4(1.0);
}