#include "Image.h"
#include "Logger/Logger.h"

namespace CEngine {
    const char *Image::TAG = "Image";

    template<typename T>
    Image::Image(const unsigned int width, const unsigned int height, T data) : Width(width), Height(height),
        mColorMode(ColorMode::NONE), Data(nullptr) {
        const unsigned int size = width * height;
        if (size == 0) {
            LogE << "图像长或宽不能为零!";
            return;
        }
        if (std::is_same<T, GRAY *>()) mColorMode = ColorMode::GRAY;
        if (std::is_same<T, GRAY_A *>()) mColorMode = ColorMode::GRAY_A;
        if (std::is_same<T, RGB *>()) mColorMode = ColorMode::RGB;
        if (std::is_same<T, RGBA *>()) mColorMode = ColorMode::RGBA;
        if (mColorMode != ColorMode::NONE) {
            Data = new T[size];
            memcpy(Data, data, sizeof(T) * size);
        } else {
            LogE << "未知数据类型!";
            return;
        }
    }

    template Image::Image(unsigned int, unsigned int, GRAY *);

    template Image::Image(unsigned int, unsigned int, GRAY_A *);

    template Image::Image(unsigned int, unsigned int, RGB *);

    template Image::Image(unsigned int, unsigned int, RGBA *);

    Image::Image(const unsigned int width, const unsigned int height, const unsigned char *buf,
                 const ColorMode mode) : Width(width), Height(height), mColorMode(mode), Data(nullptr) {
        const unsigned int size = width * height;
        if (size == 0) {
            LogE << "图像长或宽不能为零!";
            return;
        }
        if (mColorMode == ColorMode::GRAY) {
            const auto data = new GRAY[size];
            Data = data;
            for (unsigned int i = 0; i < size; i++) {
                data[i].Value = buf[i];
            }
        } else if (mColorMode == ColorMode::GRAY_A) {
            auto *data = new GRAY_A[size];
            Data = data;
            for (unsigned int i = 0; i < size; i++) {
                data[i].Value = buf[i * 2];
                data[i].Alpha = buf[i * 2 + 1];
            }
        } else if (mColorMode == ColorMode::RGB) {
            const auto data = new RGB[size];
            Data = data;
            for (unsigned int i = 0; i < size; i++) {
                data[i].R = buf[i * 3];
                data[i].G = buf[i * 3 + 1];
                data[i].B = buf[i * 3 + 2];
            }
        } else if (mColorMode == ColorMode::RGBA) {
            const auto data = new RGBA[size];
            Data = data;
            for (unsigned int i = 0; i < size; i++) {
                data[i].R = buf[i * 4];
                data[i].G = buf[i * 4 + 1];
                data[i].B = buf[i * 4 + 2];
                data[i].A = buf[i * 4 + 3];
            }
        } else {
            LogE << "未知数据类型!";
            return;
        }
    }

    Image::Image(const ImageBuffer &image_buf) {
        new(this) Image(image_buf.GetWidth(), image_buf.GetHeight(), image_buf.GetBuffer(), image_buf.GetColorMode());
    }

    Image::Image(const char *image_path) {
        new(this) Image(ImageBuffer(image_path));
    }

    Image::~Image() {
        if (!Data) return;
        if (mColorMode == ColorMode::GRAY) {
            delete[] static_cast<GRAY *>(Data);
        } else if (mColorMode == ColorMode::GRAY_A) {
            delete[] static_cast<GRAY_A *>(Data);
        } else if (mColorMode == ColorMode::RGB) {
            delete[] static_cast<RGB *>(Data);
        } else if (mColorMode == ColorMode::RGBA) {
            delete[] static_cast<RGBA *>(Data);
        } else {
            LogE << "数据存在, 但类型未定义, 无法清除!";
        }
    }

    bool Image::IsValid() const {
        if (Data) return true;
        return false;
    }

    void *Image::GetBuffer() const {
        if (mColorMode == ColorMode::GRAY) {
            return static_cast<GRAY *>(Data);
        } else if (mColorMode == ColorMode::GRAY_A) {
            return static_cast<GRAY_A *>(Data);
        } else if (mColorMode == ColorMode::RGB) {
            return static_cast<RGB *>(Data);
        } else if (mColorMode == ColorMode::RGBA) {
            return static_cast<RGBA *>(Data);
        } else {
            return nullptr;
        }
    }
}
