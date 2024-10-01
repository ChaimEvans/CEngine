#ifndef PIXEL_H
#define PIXEL_H

#include <string>

namespace CEngine {
    enum class ColorMode {
        NONE,
        GRAY,
        GRAY_A,
        RGB,
        RGBA
    };

    struct RGB {
    public:
        unsigned char R;
        unsigned char G;
        unsigned char B;

        RGB() : R(0), G(0), B(0) {
        };

        RGB(const unsigned char r, const unsigned char g, const unsigned char b) : R(r), G(g), B(b) {
        };
        explicit RGB(const char *hex_color);
        std::string ToString() const;

    private:
        const static char *TAG;
    };

    struct RGBA {
    public:
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;

        RGBA() : R(0), G(0), B(0), A(255) {
        };

        RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) {
        };

        explicit RGBA(const char *hex_color);

        RGBA(const RGB rgb, const unsigned char a) : R(rgb.R), G(rgb.G), B(rgb.G), A(a) {
        };

        RGB ToRGB() const { return {R, G, B}; };

        std::string ToString() const;

    private:
        const static char *TAG;
    };

    struct GRAY {
    public:
        unsigned char Value;

        GRAY() : Value(0) {
        };

        explicit GRAY(const unsigned char value) : Value(value) {
        };

        std::string ToString() const;

    private:
        const static char *TAG;
    };

    struct GRAY_A {
    public:
        unsigned char Value;
        unsigned char Alpha;

        GRAY_A() : Value(0), Alpha(255) {
        };

        GRAY_A(const unsigned char value, const unsigned char alpha) : Value(value), Alpha(alpha) {
        };

        std::string ToString() const;

    private:
        const static char *TAG;
    };

    struct HSV {
        // TODO
    };
}
#endif // !PIXEL_H
