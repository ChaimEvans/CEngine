/**
 * @file Texture.ixx
 * @brief 纹理贴图
 * @version 1.0
 * @author Chaim
 * @date 2024/10/05
 */

module;
#include <glad/glad.h>
#include "md5.hpp"
export module CEngine.Render:Texture;
import std;
import CEngine.Base;
import CEngine.Image;
import CEngine.Logger;
import CEngine.Utils;

namespace CEngine {
    export class Texture final : public Object {
    public:
        static const char *TAG;
        static std::unordered_map<std::string, std::shared_ptr<Texture> > All_Instances;

        static std::shared_ptr<Texture> Create(const ImageBuffer &img) {
            // 计算MD5
            const auto _md5 = md5::digestString(img.GetBuffer(), img.GetHeight() * img.GetWidth());
            if (All_Instances.contains(_md5)) return All_Instances[_md5];
            // 上传GPU
            unsigned int id = 0;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int internalFormat = GL_RGBA8;
            int dataFormat = GL_RGBA;
            switch (img.GetColorMode()) {
                case ColorMode::GRAY:
                    internalFormat = GL_R8;
                    dataFormat = GL_RED;
                    break;
                case ColorMode::GRAY_A:
                    internalFormat = GL_RG8;
                    dataFormat = GL_RG;
                    break;
                case ColorMode::RGB:
                    internalFormat = GL_RGB8;
                    dataFormat = GL_RGB;
                    break;
                case ColorMode::RGBA:
                    internalFormat = GL_RGBA8;
                    dataFormat = GL_RGBA;
                    break;
                default: break;
            }
            // internalFormat = GL_RGB;
            // dataFormat = GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(img.GetWidth()), static_cast<GLsizei>(img.GetHeight()), 0, dataFormat,
                         GL_UNSIGNED_BYTE, img.GetBuffer());
            glBindTexture(GL_TEXTURE_2D, 0);
            auto tex = std::make_shared<Texture>(id, _md5, internalFormat, dataFormat);
            All_Instances.emplace(_md5, tex);
            return tex;
        }

        static std::shared_ptr<Texture> FromFile(const char *img_path) {
            if (!Utils::FileExists(img_path)) {
                LogE(TAG) << "文件不存在: " << img_path;
                return nullptr;
            }
            const auto img = ImageBuffer(img_path);
            return Create(img);
        }

        Texture(const unsigned int id, const std::string &_md5, const int internal_format, const int data_format) : TextureID(id),
            InternalFormat(internal_format), DataFormat(data_format), Md5(_md5) {
        }

        Texture(const Texture &) = delete;
        Texture(Texture &&tex) = delete;
        Texture &operator=(Texture &&tex) = delete;
        Texture &operator=(Texture &tex) = delete;

        ~Texture() override {
            All_Instances.erase(Md5);
            glDeleteTextures(1, &TextureID);
        }

        void Bind() const {
            glBindTexture(GL_TEXTURE_2D, TextureID);
        }

        /// @property TextureID
        unsigned int getTextureID() const {
            return TextureID;
        }

        /// @property InternalFormat
        int getInternalFormat() const {
            return InternalFormat;
        }

        /// @property DataFormat
        int getDataFormat() const {
            return DataFormat;
        }

        /// @property Md5
        std::string getMd5() const {
            return Md5;
        }

    private:
        unsigned int TextureID = 0;
        int InternalFormat = GL_RGBA8;
        int DataFormat = GL_RGBA;
        std::string Md5;
    };

    const char *Texture::TAG = "Texture";
    std::unordered_map<std::string, std::shared_ptr<Texture> > Texture::All_Instances;
}
