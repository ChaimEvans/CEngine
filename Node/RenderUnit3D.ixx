/**
 * @file RenderUnit3D.ixx
 * @brief 渲染单位
 * @version 1.0
 * @author Chaim
 * @date 2024/10/05
 */

module;
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
export module CEngine.Node:RenderUnit3D;
import :Node3D;
import std;
import CEngine.Render;

namespace CEngine {
    /**
     * @brief 渲染单位
     * @remark 网格+着色器+材质
     */
    export class RenderUnit3D : public Node3D {
    public:
        const char *GetTypeName() override {
            return "RenderUnit3D";
        }

        static RenderUnit3D *Create(Mesh *m, ShaderProgram *s) {
            return new RenderUnit3D(m, s);
        }

        /**
         * 执行渲染
         */
        virtual void Render() {
            shader_program->Use();
            shader_program->SetUniform(0, GetWorldMatrix());
            if (!uniforms.empty())
                for (auto [name, value]: uniforms) {
                    shader_program->SetShaderUniformVar(name.c_str(), value);
                }
            mesh->Render();
        }

        /**
         * 设置着色器参数
         * @param name 变量名称
         * @param value 值
         */
        template<typename T>
        void SetShaderUniform(const std::string &name, T &&value) {
            LogI(TAG) << "设置Shader Uniform: " << name << " = " << value;
            if (uniforms.contains(name)) uniforms.erase(name);
            uniforms.emplace(name, ShaderUniformVar(value));
        }

        /// @property mesh
        Mesh *getMesh() const { return mesh; }

        /// @property shader_program
        ShaderProgram *getShaderProgram() const { return shader_program; }

        /// @property uniforms
        std::unordered_map<std::string, ShaderUniformVar> &getUniforms() { return uniforms; }

    protected:
        RenderUnit3D(Mesh *m, ShaderProgram *s) : mesh(m), shader_program(s) {
        }

        Mesh *mesh;
        ShaderProgram *shader_program;
        std::unordered_map<std::string, ShaderUniformVar> uniforms;
    };
}
