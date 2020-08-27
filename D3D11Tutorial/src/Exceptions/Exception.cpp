#include "pch.h"
#include "Exception.h"


namespace Except
{
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
}

