//
// Created by chaim on 24-10-14.
//

#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

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

layout (std140) uniform Material_Parameters
{
    float Emissive_Intensity;
    float Metallic;
    float Roughness;
    float Opacity;
    vec4 Diffuse_Color;
    vec4 Specular_Color;
    vec4 Emission_Color;
    vec4 Reflective_Color;
    vec4 Transparent_Color;
};

out vec3 Vertex_Position;
out vec3 Vertex_Normal;
out vec2 UV;

void main() {
    gl_Position = Transform * vec4(Position, 1.0);
    Vertex_Position = Position;
    Vertex_Normal = Normal;
    UV = TexCoord;
}