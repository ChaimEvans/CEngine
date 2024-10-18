/**
 * @file Material.ixx
 * @brief 材质(PBR)相关
 * @version 1.0
 * @author Chaim
 * @date 2024/10/06
 */

module;
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
export module CEngine.Render:Material;
import :Texture;
import :ShaderProgram;

namespace CEngine {
    export class Material {
    public:
        static Material ProcessAssimpMaterial(const aiMaterial *material, const char *model_file_path) {
            // 切割文本获取目录
            auto file_dir = std::string(model_file_path);
            if (const size_t pos = file_dir.find_last_of("/\\"); pos != std::string::npos)
                file_dir = file_dir.substr(0, pos + 1);
            Material mat;
            // 读取、上传贴图
            for (auto &[key, value]: mat.Textures) {
                if (!value.second) continue;
                if (material->GetTextureCount(key) > 0) {
                    aiString tex_path;
                    material->GetTexture(key, 0, &tex_path);
                    if (tex_path.length > 0)
                        value.first = Texture::FromFile(std::format("{}{}", file_dir, tex_path.data).c_str());
                }
            }
            // 读取参数
            material->Get(AI_MATKEY_EMISSIVE_INTENSITY, mat.Parameters.EMISSIVE_INTENSITY);
            if (mat.Textures[aiTextureType_METALNESS].first == nullptr)
                material->Get(AI_MATKEY_METALLIC_FACTOR, mat.Parameters.METALLIC);
            if (mat.Textures[aiTextureType_DIFFUSE_ROUGHNESS].first == nullptr)
                material->Get(AI_MATKEY_ROUGHNESS_FACTOR, mat.Parameters.ROUGHNESS);
            material->Get(AI_MATKEY_OPACITY, mat.Parameters.OPACITY);
            if (mat.Textures[aiTextureType_DIFFUSE].first == nullptr)
                material->Get(AI_MATKEY_COLOR_DIFFUSE, mat.Parameters.DIFFUSE_COLOR);
            if (mat.Textures[aiTextureType_SPECULAR].first == nullptr)
                material->Get(AI_MATKEY_COLOR_SPECULAR, mat.Parameters.SPECULAR_COLOR);
            if (mat.Textures[aiTextureType_EMISSION_COLOR].first == nullptr)
                material->Get(AI_MATKEY_COLOR_EMISSIVE, mat.Parameters.EMISSION_COLOR);
            if (mat.Textures[aiTextureType_REFLECTION].first == nullptr)
                material->Get(AI_MATKEY_COLOR_REFLECTIVE, mat.Parameters.REFLECTIVE_COLOR);
            material->Get(AI_MATKEY_COLOR_TRANSPARENT, mat.Parameters.TRANSPARENT_COLOR);
            // 上传参数
            glGenBuffers(1, &mat.UBO_Parameters);
            glBindBuffer(GL_UNIFORM_BUFFER, mat.UBO_Parameters);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(MParameters), &mat.Parameters, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return std::move(mat);
        }

        /**
        * { Assimp纹理类型枚举, {Texture指针, 是否启用} }
        */
        // @formatter:off
        std::unordered_map<aiTextureType, std::pair<Texture *, bool> > Textures = {
            {aiTextureType_DIFFUSE,             {nullptr, true}},   // 漫反射纹理
            {aiTextureType_SPECULAR,            {nullptr, true}},   // 镜面反射纹理
            {aiTextureType_AMBIENT,             {nullptr, false}},  // 环境光纹理
            {aiTextureType_EMISSIVE,            {nullptr, true}},   // 自发光纹理
            {aiTextureType_HEIGHT,              {nullptr, true}},   // 高度纹理
            {aiTextureType_NORMALS,             {nullptr, true}},   // 法线纹理
            {aiTextureType_SHININESS,           {nullptr, false}},  // 光泽度纹理
            {aiTextureType_OPACITY,             {nullptr, true}},   // 不透明度纹理
            {aiTextureType_DISPLACEMENT,        {nullptr, true}},   // 位移纹理
            {aiTextureType_LIGHTMAP,            {nullptr, true}},   // 光照贴图纹理
            {aiTextureType_REFLECTION,          {nullptr, true}},   // 反射纹理
            {aiTextureType_BASE_COLOR,          {nullptr, true}},   // 基础颜色纹理
            {aiTextureType_NORMAL_CAMERA,       {nullptr, false}},  // 摄像机空间法线纹理
            {aiTextureType_EMISSION_COLOR,      {nullptr, true}},   // 发射颜色纹理
            {aiTextureType_METALNESS,           {nullptr, true}},   // 金属度纹理
            {aiTextureType_DIFFUSE_ROUGHNESS,   {nullptr, true}},   // 漫反射粗糙度纹理
            {aiTextureType_AMBIENT_OCCLUSION,   {nullptr, true}},   // 环境遮蔽纹理
            {aiTextureType_SHEEN,               {nullptr, false}},  // 光泽纹理
            {aiTextureType_CLEARCOAT,           {nullptr, false}},  // 清漆纹理
            {aiTextureType_TRANSMISSION,        {nullptr, false}}   // 透射纹理
        };

        /// @remark 请据此顺序在GLSL中构建结构体
        struct MParameters {
            float EMISSIVE_INTENSITY    = 1.0f; // 发射强度
            float METALLIC              = 1.0f; // 金属度
            float ROUGHNESS             = 1.0f; // 粗糙度
            float OPACITY               = 1.0f; // 透明度
            aiColor4D DIFFUSE_COLOR     = {1.0f, 1.0f, 1.0f, 1.0f}; // 漫反射颜色
            aiColor4D SPECULAR_COLOR    = {1.0f, 1.0f, 1.0f, 1.0f}; // 镜面反射颜色
            aiColor4D EMISSION_COLOR    = {1.0f, 1.0f, 1.0f, 1.0f}; // 发射颜色
            aiColor4D REFLECTIVE_COLOR  = {1.0f, 1.0f, 1.0f, 1.0f}; // 反射颜色
            aiColor4D TRANSPARENT_COLOR = {1.0f, 1.0f, 1.0f, 1.0f}; // 透明度（无对应纹理）（不知道是啥）
        };
        // @formatter:on

        MParameters Parameters;


        /**
         * 使用材质
         * @remark 请先<code>Use</code>ShaderProgram
         * @remark 请保证GLSL中材质参数结构体与MParameters结构相同
         * @remark 请保证在第一个材质贴图
         * @param shader \n
         * ShaderProgram指针
         * @param material_parameters_uniform_block_index \n
         * 材质参数Uniform结构索引 (使用函数<code>glGetUniformBlockIndex()</code>) \n
         * 若不传参，则自动搜索结构<code>Material_Parameters</code>
         * @param uniform_tex_start_location \n
         * 材质贴图的<b>第一个</b>Uniform变量的地址 (使用函数<code>glGetUniformLocation()</code>) \n
         * 若不传参，则自动搜索<code>Tex_Diffuse</code>
         */
        void Use(const ShaderProgram *shader, const int material_parameters_uniform_block_index = -1, const int uniform_tex_start_location = -1) const {
            const auto shader_id = shader->getShaderProgramID();
            const unsigned int i = material_parameters_uniform_block_index < 0
                                       ? glGetUniformBlockIndex(shader_id, "Material_Parameters")
                                       : material_parameters_uniform_block_index;
            glUniformBlockBinding(shader_id, i, UBO_Parameters);

            /* 顺序法（需保证GLSL中uniform顺序与Textures顺序相同，且保证所有变量被使用已避免被编译器优化） */
            // int uniform_location = uniform_tex_start_location < 0
            //                            ? glGetUniformLocation(shader_id, "Tex_Diffuse")
            //                            : uniform_tex_start_location;
            // uniform_location--;
            // for (const auto &[fst, snd]: Textures | std::views::values) {
            //     uniform_location++; // 不管有没有纹理 地址都会往下走
            //     if (fst == nullptr || !snd) continue;
            //     glUniform1i(uniform_location, fst->Use());
            // }

            /* 寻址法 */
            for (auto &[type, value]: Textures) {
                if (value.first == nullptr || !value.second) return;
                const char *name = nullptr;
                // @formatter:off
                switch (type) {
                    case aiTextureType_DIFFUSE:             name = "Tex_Diffuse";           break;
                    case aiTextureType_SPECULAR:            name = "Tex_Specular";          break;
                    case aiTextureType_AMBIENT:             name = "Tex_Ambient";           break;
                    case aiTextureType_EMISSIVE:            name = "Tex_Emissive";          break;
                    case aiTextureType_HEIGHT:              name = "Tex_Height";            break;
                    case aiTextureType_NORMALS:             name = "Tex_Normals";           break;
                    case aiTextureType_SHININESS:           name = "Tex_Shininess";         break;
                    case aiTextureType_OPACITY:             name = "Tex_Opacity";           break;
                    case aiTextureType_DISPLACEMENT:        name = "Tex_Displacement";      break;
                    case aiTextureType_LIGHTMAP:            name = "Tex_Lightmap";          break;
                    case aiTextureType_REFLECTION:          name = "Tex_Reflection";        break;
                    case aiTextureType_BASE_COLOR:          name = "Tex_BaseColor";         break;
                    case aiTextureType_NORMAL_CAMERA:       name = "Tex_NormalCamera";      break;
                    case aiTextureType_EMISSION_COLOR:      name = "Tex_EmissionColor";     break;
                    case aiTextureType_METALNESS:           name = "Tex_Metalness";         break;
                    case aiTextureType_DIFFUSE_ROUGHNESS:   name = "Tex_DiffuseRoughness";  break;
                    case aiTextureType_AMBIENT_OCCLUSION:   name = "Tex_AmbientOcclusion";  break;
                    case aiTextureType_SHEEN:               name = "Tex_Sheen";             break;
                    case aiTextureType_CLEARCOAT:           name = "Tex_Clearcoat";         break;
                    case aiTextureType_TRANSMISSION:        name = "Tex_Transmission";      break;
                    default:                                name = "Unknown_Texture";       break;
                }
                if(const auto location = glGetUniformLocation(shader_id, name); location >= 0) {
                    glUniform1i(location, value.first->Use());
                }
                // @formatter:on
            }
        }

    private
    :
        unsigned int UBO_Parameters = 0;
    };
}
