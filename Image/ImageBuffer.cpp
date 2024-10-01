#include "ImageBuffer.h"
#include "Logger/Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace CEngine {
    const char *ImageBuffer::TAG = "ImageBuffer";

    ImageBuffer::ImageBuffer(const unsigned int width, const unsigned int height, const unsigned char *data,
                             const ColorMode mode) : Data(nullptr), Width(width), Height(height),
                                                     mColorMode(mode) {
        unsigned int size = width * height;
        if (size == 0) {
            LogE << "图像长或宽不能为零!";
            return;
        }
        if (mode == ColorMode::NONE) {
            LogE << "未知数据类型!";
            return;
        }
        Data = new unsigned char[size];
        memcpy(Data, data, sizeof(char) * size);
    }

    ImageBuffer::ImageBuffer(const char *img_path) : Data(nullptr), Width(0), Height(0), mColorMode(ColorMode::NONE) {
        int width = 0;
        int height = 0;
        int PicType = 0;
        unsigned char *buf = stbi_load(img_path, &width, &height, &PicType, STBI_default);
        switch (PicType) {
            case static_cast<int>(STBI_grey):
                mColorMode = ColorMode::GRAY;
                break;
            case static_cast<int>(STBI_grey_alpha):
                mColorMode = ColorMode::GRAY_A;
                break;
            case static_cast<int>(STBI_rgb):
                mColorMode = ColorMode::RGB;
                break;
            case static_cast<int>(STBI_rgb_alpha):
                mColorMode = ColorMode::RGBA;
                break;
            default: break;
        }
        new(this) ImageBuffer(width, height, buf, mColorMode);
        stbi_image_free(buf);
    }

    ImageBuffer::~ImageBuffer() {
        delete[] Data;
    }

    bool ImageBuffer::IsValid() const {
        if (Data) return true;
        return false;
    }
}
