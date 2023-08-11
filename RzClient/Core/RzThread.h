#pragma once

#include <thread>

namespace RzLib
{
    class ScopeThread
    {
    public:
        ScopeThread(std::thread&& thread);
        ~ScopeThread();

        ScopeThread(const ScopeThread& rhs) = delete;
        ScopeThread& operator=(const ScopeThread& rhs) = delete;
    private:
        std::thread m_thread;
    };
}
