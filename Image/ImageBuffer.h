#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

#include "Pixel.h"

namespace CEngine {
    class ImageBuffer {
    public:
        ImageBuffer(unsigned int width, unsigned int height, const unsigned char *data, ColorMode mode);
        explicit ImageBuffer(const char *img_path);
        ~ImageBuffer();
        bool IsValid() const;
        unsigned char *GetBuffer() const { return Data; }
        unsigned int GetWidth() const { return Width; }
        unsigned int GetHeight() const { return Height; }
        ColorMode GetColorMode() const { return mColorMode; }

    private:
        const static char *TAG;
        unsigned char *Data;
        unsigned int Width;
        unsigned int Height;
        ColorMode mColorMode;
    };
}
#endif // !IMAGEBUFFER_H
