#include "pch.h"
#include "Exception.h"

#include "dxerr.h"


namespace Except
{
    //Exception
    Exception::Exception(int line, const char* file)
        : m_Line(line), m_File(file), m_WhatBuffer("")
    {
    }

    const char* Exception::what() const
    {
        std::ostringstream oss;
        oss << GetType() << '\n'
            << GetOriginString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    const char* Exception::GetType() const
    {
        return "Tutorial Exception";
    }

    int Exception::GetLine() const
    {
        return m_Line;
    }

    const std::string& Exception::GetFile() const
    {
        return m_File;
    }
    std::string Exception::GetOriginString() const
    {
        std::ostringstream oss;
        oss << "[File] " << m_File << '\n'
            << "[Line] " << m_Line;

        return oss.str();
    }



    //WindowException
    WindowException::WindowException(int line, const char* file, HRESULT hr)
        : Except::Exception(line, file), m_Hr(hr)
    {
    }

    const char* WindowException::what() const
    {
        std::ostringstream oss;
        oss << GetType() << '\n'
            << "[Error Code] " << GetErrorCode() << '\n'
            << "[Description] " << GetErrorString() << '\n'
            << GetOriginString();

        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    const char* WindowException::GetType() const
    {
        return "Tutorial Window Exception";
    }

    std::string WindowException::TranslateErrorCode(HRESULT hr)
    {
        char* pMsgBuf = nullptr;
        DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, nullptr);

        if (nMsgLen == 0)
            return "Unidentified error code";

        std::string errorString = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorString;
    }

    HRESULT WindowException::GetErrorCode() const
    {
        return m_Hr;
    }

    std::string WindowException::GetErrorString() const
    {
        return TranslateErrorCode(m_Hr);
    }


    //Graphics Exception
    HrException::HrException(int line, const char* file, HRESULT hr)
        : Exception(line, file), m_Hr(hr)
    {

    }

    const char* HrException::what() const
    {
        std::ostringstream oss;
        oss << GetType() << '\n'
            << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
            << std::dec << " (" << (unsigned long)GetErrorCode() << ")\n"
            << "[Error String] " << GetErrorString() << '\n'
            << "[Description] " << GetErrorDescription() << '\n'
            << GetOriginString();
        m_WhatBuffer = oss.str();
        return m_WhatBuffer.c_str();
    }

    const char* HrException::GetType() const
    {
        return "Graphics Exception";
    }

    HRESULT HrException::GetErrorCode() const
    {
        return m_Hr;
    }

    std::string HrException::GetErrorString() const
    {
        return DXGetErrorString(m_Hr);
    }

    std::string HrException::GetErrorDescription() const
    {
        char buff[512];
        DXGetErrorDescription(m_Hr, buff, sizeof(buff));
        return buff;
    }

    DeviceRemovedException::DeviceRemovedException(int line, const char* file, HRESULT hr)
        : HrException(line, file, hr)
    {
    }

    const char* DeviceRemovedException::GetType() const
    {
        return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
    }

    NoGraphicsException::NoGraphicsException(int line, const char* file)
        : Exception(line, file)
    {
    }

    const char* NoGraphicsException::GetType() const
    {
        return "No Graphic Exception";
    }

}

