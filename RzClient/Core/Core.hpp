#pragma once

namespace RzLib
{
    enum class RECV_CMD
    {
        UNKNOWN = 0,
        NORMAL,         // 一指令，例如常规收发信息
        FILE,           // 三指令 file filesize filepath
        VER,            // 二指令 ver version
        update          // 服务器发给客户端，指明下面需要更新客户端了
    };
}
