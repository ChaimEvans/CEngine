#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image/ImageBuffer.h"

namespace CEngine {
    class Texture {
    public:
        explicit Texture(const ImageBuffer &img);
        explicit Texture(const char *img_path);
        ~Texture();
        void Bind() const;
        void UnBind() const;

    private:
        unsigned int TextureID = 0;
    };
}
#endif //TEXTURE_H
