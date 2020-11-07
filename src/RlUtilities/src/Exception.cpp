#include "utpch.h"
#include "Exception.h"


namespace At0::Reyal
{
    Exception::Exception(uint16_t line, const char* file)
        : m_Line(line), m_File(file), m_WhatBuffer("")
    {
    }

    std::string Exception::GetDefaultString() const
    {
        std::stringstream oss;
        oss << "[File] " << m_File << '\n'
            << "[Line] " << m_Line;

        return oss.str();
    }

    const char* Exception::what() const
    {
        std::stringstream oss;
        oss << GetType() << '\n'
            << GetDefaultString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }
    
    
    WindowsException::WindowsException(HRESULT hr, uint16_t line, const char* file)
        : Exception(line, file), m_Hr(hr)
    {
    }
    
    const char* WindowsException::what() const
    {
        std::stringstream oss;
        oss << GetType() << '\n'
            << "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
            << "[Description] " << GetErrorString() << '\n'
            << GetDefaultString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    std::string WindowsException::GetErrorString() const
    {
        char* pMsgBuf;
        DWORD msgLen = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, nullptr
        );

        if (msgLen == 0)
            return "Unidentified error code";

        std::string errorCode = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorCode;
    }
}
