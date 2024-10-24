/**
 * @file PBR3D.ixx
 * @brief 使用预设的PBRShader
 * @version 1.0
 * @author Chaim
 * @date 2024/10/05
 */

module;
#include <glm/glm.hpp>
export module CEngine.Node:PBR3D;
import :RenderUnit3D;
import CEngine.Render;

namespace CEngine {
    export class PBR3D final : public RenderUnit3D {
    public:
        const char *GetTypeName() override {
            return "PBR3D";
        }

        static PBR3D *Create(Mesh *m, Material &&mat, ShaderProgram *shader = nullptr) {
            return new PBR3D(m, std::move(mat), shader);
        }

        void Render(const glm::mat4 &viewM, const glm::mat4 &projectM) override {
            shader_program->Use();
            shader_program->SetUniform(0, projectM * (GetWorldMatrix() * viewM));
            Mat.Use(shader_program);
            if (!uniforms.empty())
                for (auto [name, value]: uniforms) {
                    shader_program->SetShaderUniformVar(name.c_str(), value);
                }
            mesh->Render();
        }

        Material &getMaterial() {
            return Mat;
        }

    protected:
        PBR3D(Mesh *m, Material &&mat, ShaderProgram *shader) : RenderUnit3D(m, shader == nullptr ? ShaderProgram::All_Instances["PBR"] : shader),
                                                                Mat(std::move(mat)) {
        }

        Material Mat;
    };
}
