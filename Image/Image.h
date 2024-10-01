#ifndef IMAGE_H
#define IMAGE_H

#include "Pixel.h"
#include "ImageBuffer.h"

namespace CEngine {
    class Image {
    public:
        template<typename T>
        Image(unsigned int width, unsigned int height, T data);
        Image(unsigned int width, unsigned int height, const unsigned char *buf, ColorMode mode);
        explicit Image(const ImageBuffer &image_buf);
        explicit Image(const char *image_path);
        ~Image();
        bool IsValid() const;
        void *GetBuffer() const;
        unsigned int GetWidth() const { return Width; }
        unsigned int GetHeight() const { return Height; }
        ColorMode GetColorMode() const { return mColorMode; }

    private:
        const static char *TAG;
        unsigned int Width{};
        unsigned int Height{};
        ColorMode mColorMode;
        void *Data{};
    };
}
#endif // !IMAGE_H
