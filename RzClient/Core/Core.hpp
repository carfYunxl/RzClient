#pragma once

namespace RzLib
{
    enum class RECV_CMD : unsigned char
    {
        NORMAL          = 0xF1,
        VERSION         = 0xF2,
        UPDATE          = 0xF4,
        FILE_HEADER     = 0xF5,
        FILE_CONTENT    = 0xF6,
        File_TAIL       = 0xF7
    };
}
