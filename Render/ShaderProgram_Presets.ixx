/**
 * @file ShaderProgram_Presets.ixx
 * @brief 着色器程序预设
 * @version 1.0
 * @author Chaim
 * @date 2024/10/11
 */

export module CEngine.Render:ShaderProgram_Presets;
import :ShaderProgram;
import :GLSL;
import std;
import CEngine.Logger;
import CEngine.Utils;

namespace CEngine {
    auto TAG = "着色器预设";
    auto presets_shader_directory = "Shader";

    void ShaderProgram::LoadPresets() {
        LogI(TAG) << "加载预设着色器...";
        if (!std::filesystem::exists(presets_shader_directory) || !std::filesystem::is_directory(presets_shader_directory)) {
            LogE(TAG) << "预设着色器文件夹不存在: " << presets_shader_directory;
            return;
        }
        for (const auto &file: std::filesystem::directory_iterator(presets_shader_directory)) {
            auto vert_path = file.path().string();
            if (!vert_path.ends_with("vert")) continue;
            auto frag_path = std::string(vert_path);
            frag_path.replace(frag_path.end() - 4, frag_path.end(), "frag");
            if (!Utils::FileExists(frag_path.c_str())) continue;
            auto _filename = file.path().filename().string();
            const auto shader_name = _filename.substr(0, _filename.find_last_of('.'));
            LogI(TAG) << "加载预设着色器: " << shader_name << " (" << vert_path << ", " << frag_path << ")";
            auto vert = GLSL::FromFile(vert_path.c_str(), GLSL::ShaderType::Vertex);
            auto frag = GLSL::FromFile(frag_path.c_str(), GLSL::ShaderType::Fragment);
            if (!vert || !frag) {
                LogE(TAG) << "预设着色器加载失败: " << shader_name << " (" << vert_path << ", " << frag_path << ")";
            }
            Create(shader_name)
                    ->AddShader(vert.get())
                    ->AddShader(frag.get())
                    ->Link();
        }
    }
}
