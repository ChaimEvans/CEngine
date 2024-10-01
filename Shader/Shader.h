#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

namespace CEngine {
    class Shader {
    public:
        Shader(const char *_vertex_glsl_path, const char *_fragment_glsl_path);

        ~Shader();

        unsigned int GetShaderProgram() const { return ShaderProgram; }

        void Use() const {
            glUseProgram(ShaderProgram);
        }

        void UnUse() const {
            glUseProgram(0);
        }

        template<typename T>
        void SetUniform(const char *name, T value);

        template<typename T>
        void SetUniform(const char *name, T value_x, T value_y);

        template<typename T>
        void SetUniform(const char *name, T value_x, T value_y, T value_z);

        template<typename T>
        void SetUniform(const char *name, T value_x, T value_y, T value_z, T value_w);

    private:
        const static char *TAG;
        unsigned int ShaderProgram;
    };
}
#endif // !SHADER_H
