//
// Created by chaim on 24-10-6.
//

module;
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>
export module CEngine.Utils;
import std.compat;

namespace CEngine::Utils {
    /**
     * @remark from: https://lowrey.me/guid-generation-in-c-11
     */
    unsigned char random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<unsigned char>(dis(gen));
    }

    /**
     * 生成指定长度的随机十六进制字符串
     * @remark from: https://lowrey.me/guid-generation-in-c-11
     * @param len 长度
     * @return 字符串
     */
    export std::string generate_hex(const unsigned int len) {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << static_cast<int>(rc);
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }

    export std::string GenerateUUID() {
        return generate_hex(32);
    }

    /**
     * assimp 4x4矩阵 转 glm 4x4矩阵
     * @param matrix aiMatrix4x4
     * @return glm::mat4
     */
    export glm::mat4 aiMatrix4x4ToGlmMat4(const aiMatrix4x4 &matrix) {
        return glm::mat4(matrix.a1, matrix.b1, matrix.c1, matrix.d1,
                         matrix.a2, matrix.b2, matrix.c2, matrix.d2,
                         matrix.a3, matrix.b3, matrix.c3, matrix.d3,
                         matrix.a4, matrix.b4, matrix.c4, matrix.d4);
    }


    /**
     * 判断一个字符串是否以指定的后缀结尾
     * @param str 指向要检查的字符串的指针
     * @param suffix 指向要检查的后缀的指针
     * @return 如果字符串以指定的后缀结尾，则返回<code>true</code>；如果字符串不是以指定的后缀结尾，或者任一参数为NULL，则返回<code>false</code>
     */
    export bool c_str_ends_with(const char *str, const char *suffix) {
        if (!str || !suffix) {
            return false;
        }
        const size_t str_len = strlen(str);
        const size_t suffix_len = strlen(suffix);
        if (suffix_len > str_len) {
            return false;
        }
        return strcmp(str + str_len - suffix_len, suffix) == 0;
    }

    /**
     * 十六进制转十六进制字符
     */
    export unsigned char HexToChar(const char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0; // 非法字符默认为0
    }

    export bool FileExists(const char *path) {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }
}