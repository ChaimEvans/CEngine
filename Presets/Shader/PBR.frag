//
// Created by chaim on 24-10-14.
//

#version 460

in vec3 Vertex_Position;
in vec3 Vertex_Normal;
in vec2 UV;

layout (location = 0) uniform mat4 Transform;

layout (location = 1) uniform sampler2D Tex_Diffuse;
layout (location = 2) uniform sampler2D Tex_Specular;
layout (location = 3) uniform sampler2D Tex_Ambient;
layout (location = 4) uniform sampler2D Tex_Emissive;
layout (location = 5) uniform sampler2D Tex_Height;
layout (location = 6) uniform sampler2D Tex_Normals;
layout (location = 7) uniform sampler2D Tex_Shininess;
layout (location = 8) uniform sampler2D Tex_Opacity;
layout (location = 9) uniform sampler2D Tex_Displacement;
layout (location = 10) uniform sampler2D Tex_Lightmap;
layout (location = 11) uniform sampler2D Tex_Reflection;
layout (location = 12) uniform sampler2D Tex_BaseColor;
layout (location = 13) uniform sampler2D Tex_NormalCamera;
layout (location = 14) uniform sampler2D Tex_EmissionColor;
layout (location = 15) uniform sampler2D Tex_Metalness;
layout (location = 16) uniform sampler2D Tex_DiffuseRoughness;
layout (location = 17) uniform sampler2D Tex_AmbientOcclusion;
layout (location = 18) uniform sampler2D Tex_Sheen;
layout (location = 19) uniform sampler2D Tex_Clearcoat;
layout (location = 20) uniform sampler2D Tex_Transmission;

out vec4 FragColor;

void main() {
    FragColor = texture(Tex_Diffuse, UV);
}