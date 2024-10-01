#include "Texture.h"
#include <glad/glad.h>
#include "Image/Pixel.h"

namespace CEngine {
    Texture::Texture(const ImageBuffer &img) {
        glGenTextures(1, &TextureID);
        Bind();
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
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(img.GetWidth()),
                     static_cast<GLsizei>(img.GetHeight()), 0, dataFormat, GL_UNSIGNED_BYTE, img.GetBuffer());
        UnBind();
    }

    Texture::Texture(const char *img_path) {
        const auto img = ImageBuffer(img_path);
        new(this) Texture(img);
    }

    void Texture::Bind() const {
        glBindTexture(GL_TEXTURE_2D, TextureID);
    }

    void Texture::UnBind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
