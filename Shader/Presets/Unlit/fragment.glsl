#version 460 core
out vec4 Color;
in vec4 VertexColor;
void main() {
    Color = VertexColor;
}