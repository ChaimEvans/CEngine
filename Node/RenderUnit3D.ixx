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
        static RenderUnit3D *Create(Mesh *m, ShaderProgram *s) {
            return new RenderUnit3D(m, s);
        }

        /**
         * 执行渲染
         */
        virtual void Render() {
            shader_program->Use();
            shader_program->SetUniform(0, getWorldMatrix());
            if (!uniforms.empty())
                for (auto [name, values]: uniforms) {
                    shader_program->SetUniform_p1(name.c_str(), values);
                }
            mesh->Render();
        }

        /**
         * 设置着色器参数
         * @param name 变量名称
         * @param args 值
         */
        template<typename... Args>
        void SetShaderUniform(const std::string &name, Args &... args) {
            std::vector<ShaderUniformVar> values = {ShaderUniformVar(args)...};
            if (uniforms.contains(name))
                uniforms[name] = values;
            uniforms.emplace(name, std::move(values));
        }

        /// @property mesh
        Mesh *getMesh() const {
            return mesh;
        }

        /// @property shader_program
        ShaderProgram *getShaderProgram() const {
            return shader_program;
        }

    protected:
        RenderUnit3D(Mesh *m, ShaderProgram *s) : mesh(m), shader_program(s) {
        }

        Mesh *mesh;
        ShaderProgram *shader_program;
        std::unordered_map<std::string, std::vector<ShaderUniformVar> > uniforms;
    };
}
