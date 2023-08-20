#pragma once

#include <string>

namespace RzLib
{
    constexpr size_t CMD_MIN_SIZE = 3;
    class TcpParser
    {
    public:
        explicit TcpParser(const char* cmd);
        ~TcpParser() = default;

        const unsigned char     GetCMD() const { return m_Cmd; }
        const std::string       GetMsg() const { return m_strMsg; }

    private:
        void Parser(const char* cmd);

    private:
        unsigned char   m_Cmd;
        std::string     m_strMsg;
    };
}
