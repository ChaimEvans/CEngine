#include "Shader.h"
#include <fstream>
#include <sstream>
#include "Logger/Logger.h"
#include "CEngine.h"

namespace CEngine {
    const char *Shader::TAG = "着色器";

    Shader::Shader(const char *_vertex_glsl_path, const char *_fragment_glsl_path) {
        LogD << "正在编译着色器...\n顶点着色器: " << _vertex_glsl_path << "\n片元着色器: " << _fragment_glsl_path;
        // 读取glsl文件内容
        std::ifstream vertex_fstream, fragment_fstream;
        vertex_fstream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        fragment_fstream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        const char *_vertex_glsl, *_fragment_glsl;
        try {
            vertex_fstream.open(_vertex_glsl_path);
            std::stringstream vertex_sstream;
            vertex_sstream << vertex_fstream.rdbuf();
            std::string temp = vertex_sstream.str();
            _vertex_glsl = temp.c_str();
        } catch (const std::ifstream::failure &e) {
            LogE << "读取glsl文件时发生错误: " << _vertex_glsl_path << "\n错误信息: " << e.what();
            Engine::GetIns()->Exit(-1);
        }
        try {
            fragment_fstream.open(_fragment_glsl_path);
            std::stringstream fragment_sstream;
            fragment_sstream << fragment_fstream.rdbuf();
            std::string temp = fragment_sstream.str();
            _fragment_glsl = temp.c_str();
        } catch (const std::ifstream::failure &e) {
            LogE << "读取glsl文件时发生错误: " << _fragment_glsl_path << "\n错误信息: " << e.what();
            Engine::GetIns()->Exit(-1);
        }
        // 编译着色器
        int success_flag = 0;
        // 顶点着色器
        auto vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &_vertex_glsl, NULL);
        glCompileShader(vertex_shader_id);
        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag) {
            char info_log[512];
            glGetShaderInfoLog(vertex_shader_id, 512, nullptr, info_log);
            LogE << "着色器编译错误: " << _vertex_glsl_path << "\nInfoLog: " << info_log;
            Engine::GetIns()->Exit(-1);
        }
        // 片元着色器
        auto fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_id, 1, &_fragment_glsl, nullptr);
        glCompileShader(fragment_shader_id);
        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success_flag);
        if (!success_flag) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader_id, 512, nullptr, info_log);
            LogE << "着色器编译错误: " << _fragment_glsl_path << "\nInfoLog: " << info_log;
            Engine::GetIns()->Exit(-1);
        }
        // 链接
        ShaderProgram = glCreateProgram();
        glAttachShader(ShaderProgram, vertex_shader_id);
        glAttachShader(ShaderProgram, fragment_shader_id);
        glLinkProgram(ShaderProgram);
        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success_flag);
        if (!success_flag) {
            char info_log[512];
            glGetProgramInfoLog(ShaderProgram, 512, nullptr, info_log);
            LogE << "着色器链接错误!\n顶点着色器: " << _vertex_glsl_path << "\n片元着色器: " << _fragment_glsl_path << "\nInfoLog: " << info_log;
            Engine::GetIns()->Exit(-1);
        }
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        LogS << "编译着色器完成: " << _vertex_glsl_path << " | " << _fragment_glsl_path;
    }

    Shader::~Shader() {
        glDeleteProgram(ShaderProgram);
    }

    template<typename T>
    void Shader::SetUniform(const char *name, T value) {
        LogD << "设置着色器Uniform. Name: " << name << "Value: " << value;
        auto location = glGetUniformLocation(ShaderProgram, name);
        if (!location) return;
        if (std::is_same<T, int>())
            glUniform1i(location, static_cast<int>(value));
        if (std::is_same<T, unsigned int>())
            glUniform1ui(location, static_cast<unsigned int>(value));
        if (std::is_same<T, float>())
            glUniform1f(location, static_cast<float>(value));
        if (std::is_same<T, double>())
            glUniform1d(location, static_cast<double>(value));
    }

    template void Shader::SetUniform(const char *, int);

    template void Shader::SetUniform(const char *, unsigned int);

    template void Shader::SetUniform(const char *, float);

    template void Shader::SetUniform(const char *, double);

    template<typename T>
    void Shader::SetUniform(const char *name, T value_x, T value_y) {
        LogD << "设置着色器Uniform. Name: " << name << "Value: " << value_x << ", " << value_y;
        auto location = glGetUniformLocation(ShaderProgram, name);
        if (!location) return;
        if (std::is_same<T, int>())
            glUniform2i(location, static_cast<int>(value_x), static_cast<int>(value_y));
        if (std::is_same<T, unsigned int>())
            glUniform2ui(location, static_cast<unsigned int>(value_x), static_cast<unsigned int>(value_y));
        if (std::is_same<T, float>())
            glUniform2f(location, static_cast<float>(value_x), static_cast<float>(value_y));
        if (std::is_same<T, double>())
            glUniform2d(location, static_cast<double>(value_x), static_cast<double>(value_y));
    }

    template void Shader::SetUniform(const char *, int, int);

    template void Shader::SetUniform(const char *, unsigned int, unsigned int);

    template void Shader::SetUniform(const char *, float, float);

    template void Shader::SetUniform(const char *, double, double);

    template<typename T>
    void Shader::SetUniform(const char *name, T value_x, T value_y, T value_z) {
        LogD << "设置着色器Uniform. Name: " << name << "Value: " << value_x << ", " << value_y << ", " << value_z;
        auto location = glGetUniformLocation(ShaderProgram, name);
        if (!location) return;
        if (std::is_same<T, int>())
            glUniform3i(location, static_cast<int>(value_x), static_cast<int>(value_y), static_cast<int>(value_z));
        if (std::is_same<T, unsigned int>())
            glUniform3ui(location, static_cast<unsigned int>(value_x), static_cast<unsigned int>(value_y),
                         static_cast<unsigned int>(value_z));
        if (std::is_same<T, float>())
            glUniform3f(location, static_cast<float>(value_x), static_cast<float>(value_y),
                        static_cast<float>(value_z));
        if (std::is_same<T, double>())
            glUniform3d(location, static_cast<double>(value_x), static_cast<double>(value_y),
                        static_cast<double>(value_z));
    }

    template void Shader::SetUniform(const char *, int, int, int);

    template void Shader::SetUniform(const char *, unsigned int, unsigned int, unsigned int);

    template void Shader::SetUniform(const char *, float, float, float);

    template void Shader::SetUniform(const char *, double, double, double);

    template<typename T>
    void Shader::SetUniform(const char *name, T value_x, T value_y, T value_z, T value_w) {
        LogD << "设置着色器Uniform. Name: " << name << "Value: " << value_x << ", " << value_y << ", " << value_z << ", " <<
                value_w;
        auto location = glGetUniformLocation(ShaderProgram, name);
        if (!location) return;
        if (std::is_same<T, int>())
            glUniform4i(location, static_cast<int>(value_x), static_cast<int>(value_y), static_cast<int>(value_z),
                        static_cast<int>(value_w));
        if (std::is_same<T, unsigned int>())
            glUniform4ui(location, static_cast<unsigned int>(value_x), static_cast<unsigned int>(value_y),
                         static_cast<unsigned int>(value_z), static_cast<unsigned int>(value_w));
        if (std::is_same<T, float>())
            glUniform4f(location, static_cast<float>(value_x), static_cast<float>(value_y), static_cast<float>(value_z),
                        static_cast<float>(value_w));
        if (std::is_same<T, double>())
            glUniform4d(location, static_cast<double>(value_x), static_cast<double>(value_y),
                        static_cast<double>(value_z), static_cast<double>(value_w));
    }

    template void Shader::SetUniform(const char *, int, int, int, int);

    template void Shader::SetUniform(const char *, unsigned int, unsigned int, unsigned int, unsigned int);

    template void Shader::SetUniform(const char *, float, float, float, float);

    template void Shader::SetUniform(const char *, double, double, double, double);
}
