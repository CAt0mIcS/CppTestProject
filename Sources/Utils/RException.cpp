#include "RException.h"
#include "RString.h"
#include <sstream>

namespace At0::VulkanTesting
{
	Exception::Exception(uint16_t line, const char* file) : m_File(file), m_Line(line) {}

	std::string Exception::GetDefaultString() const
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << '\n' << "[Line] " << m_Line;

		return oss.str();
	}

	const char* Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n' << GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	RuntimeException::RuntimeException(const char* message, uint16_t line, const char* file)
		: Exception(line, file), m_Message(message)
	{
	}

	const char* RuntimeException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n' << "[Description] " << m_Message << '\n' << GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	VulkanException::VulkanException(
		const char* message, uint16_t line, const char* file, VkResult result)
		: RuntimeException(message, line, file), m_Error(result)
	{
	}
	const char* VulkanException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n'
			<< "[Error Code] " << String::Construct(m_Error) << "(" << (int32_t)m_Error << ")"
			<< '\n'
			<< "[Description] " << m_Message << '\n'
			<< GetDefaultString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
}  // namespace At0::VulkanTesting
