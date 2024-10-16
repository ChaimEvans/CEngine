/**
* @file ModelImporter.ixx
 * @brief 模型导入器
 * @version 1.0
 * @author Chaim
 * @date 2024/10/06
 */

module;
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext/matrix_transform.hpp>
export module CEngine.ModelImporter;
import std;
import CEngine.Logger;
import CEngine.Render;
import CEngine.Node;
import CEngine.Utils;

namespace CEngine::ModelImporter {
    auto TAG = "ModelImporter";

    void process_node(const aiNode *node, const aiScene *scene, Node3D *parent, ShaderProgram *shader_program, const char *model_path,
                      const float transform_scale = 1.0f) {
        auto n3d = Node3D::Create();
        n3d->setName(node->mName.data);
        if (transform_scale == 1.0f)
            n3d->SetModelMatrix(Utils::aiMatrix4x4ToGlmMat4(node->mTransformation));
        else
            n3d->SetModelMatrix(glm::scale(Utils::aiMatrix4x4ToGlmMat4(node->mTransformation), glm::vec3(transform_scale)));
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            std::vector<VertexInfo> vertices;
            std::vector<unsigned int> indices;
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                VertexInfo vertex;
                vertex.Position = {mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z};
                vertex.Normal = {mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z};
                if (mesh->mTextureCoords[0])
                    vertex.TexCoord = {mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y};
                else
                    vertex.TexCoord = {0.0f, 0.0f};
                vertices.push_back(vertex);
            }
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                const aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++)
                    indices.push_back(face.mIndices[k]);
            }
            LogD(TAG) << "mMaterialIndex: " << mesh->mMaterialIndex;
            const auto m = Mesh::Create(vertices, indices);
            m->Name = mesh->mName.data;
            LogS(TAG) << "导入网格: " << m->Name;
            const auto ru = PBR3D::Create(m, Material::ProcessAssimpMaterial(scene->mMaterials[mesh->mMaterialIndex], model_path));
            n3d->AddChild(ru);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            process_node(node->mChildren[i], scene, n3d, shader_program, model_path);
        }
        parent->AddChild(std::move(n3d));
    }

    export Node3D *import_model(const char *file_path, ShaderProgram *shader_program = nullptr, float transform_scale = 1.0f) {
        if (!std::filesystem::exists(file_path)) {
            LogE(TAG) << "文件不存在: " << file_path;
            return nullptr;
        }
        LogI(TAG) << "开始导入模型: " << file_path;
        if (Utils::c_str_ends_with(file_path, ".fbx")) transform_scale = 0.1f;
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
        if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LogE(TAG) << importer.GetErrorString();
            return nullptr;
        }
        const auto node = Node3D::Create();
        node->setName(scene->mName.data);
        process_node(scene->mRootNode, scene, node, shader_program == nullptr ? ShaderProgram::All_Instances["PBR"] : shader_program, file_path,
                     transform_scale);
        return node;
    }
}
