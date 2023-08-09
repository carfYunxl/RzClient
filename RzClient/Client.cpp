#include "RzClient/RzClient.h"
#include "Core/Log.hpp"

int main()
{
    RzLib::RzClient client("192.168.2.9",8080);

    if (!client.Init())
    {
        RzLib::Log(RzLib::LogLevel::ERR,"client init error, error code : ", WSAGetLastError());
        return 0;
    }

    if (!client.Connect())
    {
        RzLib::Log(RzLib::LogLevel::ERR, "client connect error, error code : ", WSAGetLastError());
        return 0;
    }

    return 0;
}
