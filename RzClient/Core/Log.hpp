#pragma once

#include <iostream>

namespace RzLib
{
    enum class LogLevel
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERR
    };

    template<typename LogLevel, typename...Args>
    void Log(LogLevel level, Args...args)
    {
        switch (level)
        {
            case LogLevel::DEBUG:
                std::cout << "[DEBUG]";
                break;
            case LogLevel::INFO:
                std::cout << "[INFO]";
                break;
            case LogLevel::WARN:
                std::cout << "[WARN]";
                break;
            case LogLevel::ERR:
                std::cout << "[ERR]";
                break;
        }

        (std::cout << ... << args) << std::endl;
    }

    template<typename...Args>
    void print(Args...args)
    {
        (std::cout << ... << args) << std::endl;
    }
}

