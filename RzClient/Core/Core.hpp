#pragma once

namespace RzLib
{
    enum class RECV_CMD
    {
        UNKNOWN = 0,
        NORMAL,         // һָ����糣���շ���Ϣ
        FILE,           // ��ָ�� file filesize filepath
        VER,            // ��ָ�� ver version
        update          // �����������ͻ��ˣ�ָ��������Ҫ���¿ͻ�����
    };
}
