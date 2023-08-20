#include "TcpParser.h"
#include "Core/Log.hpp"

namespace RzLib
{
    TcpParser::TcpParser(const char* cmd)
        : m_Cmd(0)
        , m_strMsg("")
    {
        Parser(cmd);
    }

    void TcpParser::Parser(const char* cmd)
    {
        m_Cmd = cmd[0];

        int size = cmd[1] | (cmd[2] << 8);

        if (size > 0)
        {
            if (m_Cmd == 0xF2)  // version
            {
                m_strMsg = std::to_string(cmd[3] | cmd[4] << 8);
            }
            else
            {
                memcpy(&m_strMsg[0], &cmd[3], size);
            }
        }
    }
}
