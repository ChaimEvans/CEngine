#ifndef LOGGER_H
#define LOGGER_H
#include <string>

#define DEBUG_MODE

#define LogD Logger(Logger::LogLevel::D, TAG)
#define LogI Logger(Logger::LogLevel::I, TAG)
#define LogW Logger(Logger::LogLevel::W, TAG)
#define LogS Logger(Logger::LogLevel::S, TAG)

#ifdef DEBUG_MODE
#define LogE Logger(Logger::LogLevel::E, TAG) << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")"; Logger(Logger::LogLevel::E, TAG)
#elif
#define LogE Logger(Logger::LogLevel::E, TAG)
#endif // DEBUG_MODE

namespace CEngine {
    class Logger {
    public:
        struct Color {
            const static char *FONT_BLACK;
            const static char *FONT_RED;
            const static char *FONT_GREEN;
            const static char *FONT_YELLOW;
            const static char *FONT_BLUE;
            const static char *FONT_WHITE;
            const static char *FONT_GRAY;
            const static char *FONT_DEFAULT;

            const static char *BACKGROUND_BLACK;
            const static char *BACKGROUND_RED;
            const static char *BACKGROUND_RED_2;
            const static char *BACKGROUND_GREEN;
            const static char *BACKGROUND_GREEN_2;
            const static char *BACKGROUND_YELLOW;
            const static char *BACKGROUND_YELLOW_2;
            const static char *BACKGROUND_BLUE;
            const static char *BACKGROUND_BLUE_2;
            const static char *BACKGROUND_WHITE;
            const static char *BACKGROUND_GRAY;
            const static char *BACKGROUND_DEFAULT;
        };

        enum class LogLevel {
            D,
            I,
            W,
            E,
            S
        };

        Logger() : Logger(LogLevel::I, "日志") {
        }

        Logger(LogLevel ll, const char *TAG);

        ~Logger() = default;

        template<typename T>
        Logger &operator<<(T value);

        const static char *RestColorCode;

        static std::string MakeColorCode(const char *font, const char *bg);

    private:
        LogLevel level;
    };
}


#endif // !LOGGER_H
