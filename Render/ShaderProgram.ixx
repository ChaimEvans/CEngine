/**
 * @file ShaderProgram.ixx
 * @brief 着色器程序
 * @version 1.0
 * @author Chaim
 * @date 2024/10/05
 */

module;
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
export module CEngine.Render:ShaderProgram;
import :GLSL;
import std;
import CEngine.Base;
import CEngine.Logger;
import CEngine.Utils;

namespace CEngine {
    export class ShaderUniformVar {
    public:
        using shader_uniforms_var = std::variant<int, unsigned, float, double, glm::mat3, glm::mat4>;

        enum class Type : char {
            INT,
            UINT,
            FLOAT,
            DOUBLE,
            MAT3,
            MAT4
        };

        template<typename T>
        explicit ShaderUniformVar(const T &value) {
            Value = value;
            if constexpr (std::is_same_v<T, int>)
                type = Type::INT;
            else if constexpr (std::is_same_v<T, unsigned int>)
                type = Type::UINT;
            else if constexpr (std::is_same_v<T, float>)
                type = Type::FLOAT;
            else if constexpr (std::is_same_v<T, double>)
                type = Type::DOUBLE;
            else if constexpr (std::is_same_v<T, glm::mat3>)
                type = Type::MAT3;
            else if constexpr (std::is_same_v<T, glm::mat4>)
                type = Type::MAT4;
            else static_assert(false, "ShaderUniformVars: 未适配类型");
        }

        Type GetType() const {
            return type;
        }

        shader_uniforms_var &GetValue() {
            return Value;
        }

    private:
        Type type;
        shader_uniforms_var Value;
    };

    export class ShaderProgram final : public Object {
    public:
        const static char *TAG;
        static std::unordered_map<std::string, ShaderProgram *> All_Instances;


        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        ShaderProgram(ShaderProgram &&) = delete;
        ShaderProgram &operator=(ShaderProgram &&) = delete;

        ~ShaderProgram() override {
            glDeleteProgram(shader_program_id);
            All_Instances.erase(Name);
        }

        static void LoadPresets();

        /**
         * 创建ShaderProgram(自动生成Name)
         * @return 返回指针
         */
        static ShaderProgram *Create() {
            return new ShaderProgram();
        }

        /**
         * 创建ShaderProgram
         * @param name 名称(可通过All_Instances获取实例)
         * @return 返回指针
         */
        static ShaderProgram *Create(const std::string &name) {
            return new ShaderProgram(name);
        }

        /**
         * 添加Shader
         * @param shader GLSL对象
         * @return <code>self</code>可链式调用
         */
        ShaderProgram *AddShader(const GLSL *shader) {
            glsl_list.push_back(shader->getName());
            glAttachShader(shader_program_id, shader->getShaderID());
            return this;
        }

        /**
         * 链接所有Shader
         */
        ShaderProgram *Link() {
            glLinkProgram(shader_program_id);
            int success_flag = 0;
            glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success_flag);
            if (success_flag == GL_FALSE) {
                char info_log[512];
                glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log);
                auto logger = LogE(TAG);
                logger << "着色器链接错误! " << Name << " (";
                for (const auto file: glsl_list) {
                    logger << file << ", ";
                }
                logger << "\b\b) \n";
                logger << "InfoLog: " << info_log;
                delete this;
                return nullptr;
            }
            auto logger = LogS(TAG);
            logger << "着色器链接成功: " << Name << " (";
            for (const auto file: glsl_list) {
                logger << file << ", ";
            }
            logger << "\b\b) ";
            All_Instances[Name] = this;
            return this;
        }

        void Use() const {
            glUseProgram(shader_program_id);
        }

        template<typename T>
        void SetUniform(const char *name, const T &value) {
            SetUniform(glGetUniformLocation(shader_program_id, name), value);
        }

        template<typename T>
        void SetUniform(const int location, const T &value) {
            if (location < 0) return;
            if constexpr (std::is_same_v<T, int>)
                glUniform1i(location, value);
            else if constexpr (std::is_same_v<T, int[]>)
                glUniform1iv(location, sizeof(value) / sizeof(T), &value[0]);
            else if constexpr (std::is_same_v<T, unsigned int>)
                glUniform1ui(location, value);
            else if constexpr (std::is_same_v<T, unsigned int[]>)
                glUniform1uiv(location, sizeof(value) / sizeof(T), &value[0]);
            else if constexpr (std::is_same_v<T, float>)
                glUniform1f(location, value);
            else if constexpr (std::is_same_v<T, float[]>)
                glUniform1fv(location, sizeof(value) / sizeof(T), &value[0]);
            else if constexpr (std::is_same_v<T, double>)
                glUniform1d(location, value);
            else if constexpr (std::is_same_v<T, double[]>)
                glUniform1dv(location, sizeof(value) / sizeof(T), &value[0]);
            else if constexpr (std::is_same_v<T, glm::mat3>)
                glUniformMatrix3fv(location, 1, false, reinterpret_cast<const GLfloat *>(glm::value_ptr(value)));
            else if constexpr (std::is_same_v<T, glm::mat4>) {
                glUniformMatrix4fv(location, 1, false, reinterpret_cast<const GLfloat *>(glm::value_ptr(value)));
            } else static_assert(false, "SetUniform未实现该类型");
        }

        template<typename T>
        void SetUniform(const char *name, const T &value_x, const T &value_y) {
            SetUniform(glGetUniformLocation(shader_program_id, name), value_x, value_y);
        }

        template<typename T>
        void SetUniform(const int location, const T &value_x, const T &value_y) {
            if (location < 0) return;
            if constexpr (std::is_same_v<T, int>)
                glUniform2i(location, value_x, value_y);
            else if constexpr (std::is_same_v<T, unsigned int>)
                glUniform2ui(location, value_x, value_y);
            else if constexpr (std::is_same_v<T, float>)
                glUniform2f(location, value_x, value_y);
            else if constexpr (std::is_same_v<T, double>)
                glUniform2d(location, value_x, value_y);
            else static_assert(false, "SetUniform未实现该类型");
        }

        template<typename T>
        void SetUniform(const char *name, const T &value_x, const T &value_y, const T &value_z) {
            SetUniform(glGetUniformLocation(shader_program_id, name), value_x, value_y, value_z);
        }

        template<typename T>
        void SetUniform(const int location, const T &value_x, const T &value_y, const T &value_z) {
            if (location < 0) return;
            if constexpr (std::is_same_v<T, int>)
                glUniform3i(location, value_x, value_y, value_z);
            else if constexpr (std::is_same_v<T, unsigned int>)
                glUniform3ui(location, value_x, value_y, value_z);
            else if constexpr (std::is_same_v<T, float>)
                glUniform3f(location, value_x, value_y, value_z);
            else if constexpr (std::is_same_v<T, double>)
                glUniform3d(location, value_x, value_y, value_z);
            else static_assert(false, "SetUniform未实现该类型");
        }

        template<typename T>
        void SetUniform(const char *name, const T &value_x, const T &value_y, const T &value_z, const T &value_w) {
            SetUniform(glGetUniformLocation(shader_program_id, name), value_x, value_y, value_z, value_w);
        }

        template<typename T>
        void SetUniform(const int location, const T &value_x, const T &value_y, const T &value_z, const T &value_w) {
            if (location < 0) return;
            if constexpr (std::is_same_v<T, int>)
                glUniform4i(location, value_x, value_y, value_z, value_w);
            else if constexpr (std::is_same_v<T, unsigned int>)
                glUniform4ui(location, value_x, value_y, value_z, value_w);
            else if constexpr (std::is_same_v<T, float>)
                glUniform4f(location, value_x, value_y, value_z, value_w);
            else if constexpr (std::is_same_v<T, double>)
                glUniform4d(location, value_x, value_y, value_z, value_w);
            else static_assert(false, "SetUniform未实现该类型");
        }

        void SetUniform_p1(const char *name, std::vector<ShaderUniformVar> &uniforms) {
            if (const auto type = uniforms[0].GetType(); type == ShaderUniformVar::Type::INT)
                SetUniform_p2<int>(name, uniforms);
            else if (type == ShaderUniformVar::Type::UINT)
                SetUniform_p2<unsigned int>(name, uniforms);
            else if (type == ShaderUniformVar::Type::FLOAT)
                SetUniform_p2<float>(name, uniforms);
            else if (type == ShaderUniformVar::Type::DOUBLE)
                SetUniform_p2<double>(name, uniforms);
            else if (type == ShaderUniformVar::Type::MAT3)
                SetUniform(name, std::get<glm::mat3>(uniforms[0].GetValue()));
            else if (type == ShaderUniformVar::Type::MAT4)
                SetUniform(name, std::get<glm::mat4>(uniforms[0].GetValue()));
        }

        template<typename T>
        void SetUniform_p2(const char *name, std::vector<ShaderUniformVar> &uniforms) {
            std::vector<T> values;
            for (auto &uniform: uniforms) {
                values.push_back(std::get<T>(uniform.GetValue()));
            }
            auto count = values.size();
            if (count == 1)
                SetUniform(name, values[0]);
            else if (count == 2)
                SetUniform(name, values[0], values[1]);
            else if (count == 3)
                SetUniform(name, values[0], values[1], values[2]);
            else if (count == 4)
                SetUniform(name, values[0], values[1], values[2], values[3]);
        }

        /// @property shader_program_id
        unsigned int getShaderProgramID() const {
            return shader_program_id;
        }

    private:
        ShaderProgram() : ShaderProgram(Utils::GenerateUUID()) {
        }

        explicit ShaderProgram(const std::string &name) {
            shader_program_id = glCreateProgram();
            Name = name;
        }

        /// @brief 着色器程序ID
        unsigned int shader_program_id = 0;
        /// @brief 用于通过All_Instances调用的key
        std::string Name;
        /// @brief 该着色器程序所链接的GLSL，仅用于调试输出
        std::vector<std::string> glsl_list;
    };

    const char *ShaderProgram::TAG = "ShaderProgram";
    std::unordered_map<std::string, ShaderProgram *> ShaderProgram::All_Instances;
}
