/**
 * @file PBR3D.ixx
 * @brief 使用预设的PBRShader
 * @version 1.0
 * @author Chaim
 * @date 2024/10/05
 */

export module CEngine.Node:PBR3D;
import :RenderUnit3D;
import CEngine.Render;

namespace CEngine {
    export class PBR3D final : public RenderUnit3D {
    public:
        static PBR3D *Create(Mesh *m, Material &&mat) {
            return new PBR3D(m, std::move(mat));
        }

        void Render() override {
            shader_program->Use();
            shader_program->SetUniform(0, GetWorldMatrix());
            Mat.Use(shader_program);
            mesh->Render();
        }

        const char *GetTypeName() override {
            return "PBR3D";
        }

        template<typename... Args>
        void SetShaderUniform(const std::string &name, Args &... args) = delete;

        Material &getMaterial() {
            return Mat;
        }

    protected:
        PBR3D(Mesh *m, Material &&mat) : RenderUnit3D(m, ShaderProgram::All_Instances["PBR"]), Mat(std::move(mat)) {
        }

        Material Mat;
    };
}
