#include "Pixel.h"
#include "Utils/Utils.h"
#include "Logger/Logger.h"
#include <sstream>
#include <regex>
#include <cstring>

namespace CEngine {
    const std::regex HexColorPattern(R"(^#([A-Fa-f0-9]{6})([A-Fa-f0-9]{2})?$)");

    const char *RGB::TAG = "RGB";

    RGB::RGB(const char *hex_color) {
        if (std::regex_match(hex_color, HexColorPattern)) {
            R = Utils::HexToChar(hex_color[1]) * 16 + Utils::HexToChar(hex_color[2]);
            G = Utils::HexToChar(hex_color[3]) * 16 + Utils::HexToChar(hex_color[4]);
            B = Utils::HexToChar(hex_color[5]) * 16 + Utils::HexToChar(hex_color[6]);
        } else {
            LogE << "十六进制颜色解析错误: " << hex_color;
            new(this) RGB();
        }
    }

    std::string RGB::ToString() const {
        std::stringstream ss;
        ss << "RGB(" << static_cast<int>(R) << "," << static_cast<int>(G) << "," << static_cast<int>(B) << ")";
        return ss.str();
    }

    const char *RGBA::TAG = "RGBA";

    RGBA::RGBA(const char *hex_color) {
        if (std::regex_match(hex_color, HexColorPattern)) {
            R = Utils::HexToChar(hex_color[1]) * 16 + Utils::HexToChar(hex_color[2]);
            G = Utils::HexToChar(hex_color[3]) * 16 + Utils::HexToChar(hex_color[4]);
            B = Utils::HexToChar(hex_color[5]) * 16 + Utils::HexToChar(hex_color[6]);
            if (strlen(hex_color) == 9)
                A = Utils::HexToChar(hex_color[7]) * 16 + Utils::HexToChar(hex_color[8]);
            else
                A = 255;
        } else {
            LogE << "十六进制颜色解析错误: " << hex_color;
            new(this) RGBA();
        }
    }

    std::string RGBA::ToString() const {
        std::stringstream ss;
        ss << "RGBA(" << static_cast<int>(R) << "," << static_cast<int>(G) << "," << static_cast<int>(B) << "," <<
                static_cast<int>(A) << ")";
        return ss.str();
    }

    const char *GRAY::TAG = "灰度";

    std::string GRAY::ToString() const {
        std::stringstream ss;
        ss << "灰度(" << static_cast<int>(Value) << ")";
        return ss.str();
    }

    const char *GRAY_A::TAG = "灰度&Alpha";

    std::string GRAY_A::ToString() const {
        std::stringstream ss;
        ss << "灰度&Alpha(" << static_cast<int>(Value) << "," << static_cast<int>(Alpha) << ")";
        return ss.str();
    }
}
