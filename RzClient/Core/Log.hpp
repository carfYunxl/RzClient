#pragma once

#include <iostream>

namespace RzLib
{
    static const char black[] = { 0x1b, '[', '1', ';', '3', '0', 'm', 0 };
    static const char red[] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
    static const char yellow[] = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
    static const char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
    static const char normal[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };

    enum class LogLevel
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERR
    };

    template<typename LogLevel, typename...Args>
    void Log(LogLevel level, const Args&...args)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            std::cout << blue;
            break;
        case LogLevel::INFO:
            std::cout << red;
            break;
        case LogLevel::WARN:
            std::cout << yellow;
            break;
        case LogLevel::ERR:
            std::cout << normal;
            break;
        }

        (std::cout << ... << args) << std::endl;
    }
}

