#pragma once

#include <string>

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{

	class String
	{
	public:
		String() = delete;

		/**
		 * Constructs a string from a Vulkan function result
		 * @param res The result code
		 * @returns The string representation of the resulting code
		 */
		static std::string Construct(VkResult res);

		/**
		 * Converts a string from UTF16 to UTF8
		 * @param string The string to convert
		 * @returns The converted string
		 */
		static std::string ConvertUtf8(std::wstring_view string);

		/**
		 * Converts a char from UTF18 to UTF8
		 * @param ch The character to convert
		 * @returns The converted character
		 */
		static char ConvertUtf8(wchar_t ch);

		/**
		 * Converts a UTF16 string to a UTF8 string
		 * @param string The string to convert
		 * @returns The converted string
		 */
		static std::wstring ConvertUtf16(std::string_view string);

		/**
		 * Converts a UTF16 char to a UTF8 char
		 * @param string The char to convert
		 * @returns The converted char
		 */
		static wchar_t ConvertUtf16(char ch);

		static std::string ReplaceFirst(
			std::string str, std::string_view token, std::string_view to);
		static std::vector<std::string> Split(std::string_view str, char sep);

		/**
		 * Gets if a string starts with a token
		 * @param str The string
		 * @param token the supposed start of the string
		 * @returns If a string starts with the token
		 */
		static bool StartsWith(std::string_view str, std::string_view token);

		/**
		 * Gets if a string ends with a token
		 * @param str The string
		 * @param token the supposed end of the string
		 * @returns If a string ends with the token
		 */
		static bool EndsWith(std::string_view str, std::string_view token);

		/**
		 * Gets if a string contains another string
		 * @param str The string
		 * @param token The string that should be conained in str
		 * @returns If a string contains a token
		 */
		static bool Contains(std::string_view str, std::string_view token);
	};
}  // namespace At0::VulkanTesting
