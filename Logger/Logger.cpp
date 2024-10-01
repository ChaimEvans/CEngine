#include "Logger.h"
#include <iostream>
#include <sstream>

namespace CEngine {
    const char *Logger::Color::FONT_BLACK = "30";
    const char *Logger::Color::FONT_RED = "91";
    const char *Logger::Color::FONT_GREEN = "92";
    const char *Logger::Color::FONT_YELLOW = "93";
    const char *Logger::Color::FONT_BLUE = "34";
    const char *Logger::Color::FONT_WHITE = "97";
    const char *Logger::Color::FONT_GRAY = "37";
    const char *Logger::Color::FONT_DEFAULT = "39";

    const char *Logger::Color::BACKGROUND_BLACK = "40";
    const char *Logger::Color::BACKGROUND_RED = "41";
    const char *Logger::Color::BACKGROUND_RED_2 = "101";
    const char *Logger::Color::BACKGROUND_GREEN = "42";
    const char *Logger::Color::BACKGROUND_GREEN_2 = "102";
    const char *Logger::Color::BACKGROUND_YELLOW = "43";
    const char *Logger::Color::BACKGROUND_YELLOW_2 = "103";
    const char *Logger::Color::BACKGROUND_BLUE = "44";
    const char *Logger::Color::BACKGROUND_BLUE_2 = "106";
    const char *Logger::Color::BACKGROUND_WHITE = "47";
    const char *Logger::Color::BACKGROUND_GRAY = "100";
    const char *Logger::Color::BACKGROUND_DEFAULT = "49";

    const char *Logger::RestColorCode = "\033[0m";

    Logger::Logger(LogLevel ll, const char *TAG) : level(ll) {
        switch (ll) {
            case Logger::LogLevel::D:
#ifdef DEBUG_MODE
                std::cout << "\n" << MakeColorCode(Color::FONT_WHITE, Color::BACKGROUND_GRAY) << " 调试 " <<
                        MakeColorCode(Color::FONT_BLACK, Color::BACKGROUND_WHITE) << " " << TAG << " " << MakeColorCode(
                            Color::FONT_GRAY, Color::BACKGROUND_DEFAULT) << " ";
#endif // DEBUG_MODE
                break;
            case Logger::LogLevel::I:
                std::cout << "\n" << MakeColorCode(Color::FONT_WHITE, Color::BACKGROUND_BLUE) << " 信息 " <<
                        MakeColorCode(Color::FONT_BLACK, Color::BACKGROUND_BLUE_2) << " " << TAG << " " << MakeColorCode(
                            Color::FONT_BLUE, Color::BACKGROUND_DEFAULT) << " ";
                break;
            case Logger::LogLevel::W:
                std::cout << "\n" << MakeColorCode(Color::FONT_WHITE, Color::BACKGROUND_YELLOW) << " 警告 " <<
                        MakeColorCode(Color::FONT_BLACK, Color::BACKGROUND_YELLOW_2) << " " << TAG << " " << MakeColorCode(
                            Color::FONT_YELLOW, Color::BACKGROUND_DEFAULT) << " ";
                break;
            case Logger::LogLevel::E:
                std::cerr << "\n" << MakeColorCode(Color::FONT_WHITE, Color::BACKGROUND_RED) << " 错误 " <<
                        MakeColorCode(Color::FONT_BLACK, Color::BACKGROUND_RED_2) << " " << TAG << " " << MakeColorCode(
                            Color::FONT_RED, Color::BACKGROUND_DEFAULT) << " ";
                break;
            case Logger::LogLevel::S:
                std::cout << "\n" << MakeColorCode(Color::FONT_WHITE, Color::BACKGROUND_GREEN) << " 成功 " <<
                        MakeColorCode(Color::FONT_BLACK, Color::BACKGROUND_GREEN_2) << " " << TAG << " " << MakeColorCode(
                            Color::FONT_GREEN, Color::BACKGROUND_DEFAULT) << " ";
                break;
        }
    }

    std::string Logger::MakeColorCode(const char *font, const char *bg) {
        std::stringstream ss;
        ss << "\033[" << font << ";" << bg << "m";
        return ss.str();
    }

    template<typename T>
    Logger &Logger::operator<<(T value) {
        if (level == LogLevel::E)
            std::cerr << value;
#ifndef DEBUG_MODE
		else if (level == LogLevel::D)
			return *this;
#endif // !DEBUG_MODE
        else
            std::cout << value;
        std::fflush(stdout);
        return *this;
    }

    template Logger &Logger::operator<<(int);
    template Logger &Logger::operator<<(unsigned int);
    template Logger &Logger::operator<<(float);
    template Logger &Logger::operator<<(double);
    template Logger &Logger::operator<<(long);
    template Logger &Logger::operator<<(char *);
    template Logger &Logger::operator<<(const char *);
    template Logger &Logger::operator<<(std::string);
}
