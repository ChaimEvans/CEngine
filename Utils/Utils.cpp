#include "Utils.h"

namespace Utils {
    unsigned char HexToChar(const char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0; // 非法字符默认为0
    }
}