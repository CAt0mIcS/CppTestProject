#include "RString.h"

#include <locale>
#include <codecvt>

#include <vulkan/vulkan_core.h>


namespace At0::VulkanTesting
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_UtfConverter;

	std::string String::Construct(VkResult res)
	{
		switch (res)
		{
		case VK_SUCCESS: return "Vulkan Success.";
		case VK_NOT_READY: return "Vulkan not ready.";
		case VK_TIMEOUT: return "Vulkan timeout.";
		case VK_EVENT_SET: return "Vulkan event set.";
		case VK_EVENT_RESET: return "Vulkan event reset.h";
		case VK_INCOMPLETE: return "Vulkan incomplete.";
		case VK_ERROR_OUT_OF_HOST_MEMORY: return "Vulkan out of host memory.";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "Vulkan out of device memory.";
		case VK_ERROR_INITIALIZATION_FAILED: return "Vulkan initialization failed.";
		case VK_ERROR_DEVICE_LOST: return "Vulkan device lost.";
		case VK_ERROR_MEMORY_MAP_FAILED: return "Vulkan memory map failed.";
		case VK_ERROR_LAYER_NOT_PRESENT: return "Vulkan layer not present.";
		case VK_ERROR_EXTENSION_NOT_PRESENT: return "Vulkan extension not present.";
		case VK_ERROR_FEATURE_NOT_PRESENT: return "Vulkan feature not present.";
		case VK_ERROR_INCOMPATIBLE_DRIVER: return "Vulkan incompatible driver.";
		case VK_ERROR_TOO_MANY_OBJECTS: return "Vulkan too many objects.";
		case VK_ERROR_FORMAT_NOT_SUPPORTED: return "Vulkan format not supported.";
		case VK_ERROR_FRAGMENTED_POOL: return "Vulkan fragmented pool.";
		case VK_ERROR_UNKNOWN: return "Vulkan error unknown.";
		case VK_ERROR_OUT_OF_POOL_MEMORY: return "Vulkan out of pool memory.";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "Vulkan invalid external handle.";
		case VK_ERROR_FRAGMENTATION: return "Vulkan fragmentation error.";
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
			return "Vulkan invalid opaque capture address.";
		case VK_ERROR_SURFACE_LOST_KHR: return "Vulkan surface lost.";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "Vulkan native window in use.";
		case VK_SUBOPTIMAL_KHR: return "Vulkan suboptimal.";
		case VK_ERROR_OUT_OF_DATE_KHR: return "Vulkan out of date.";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "Vulkan incompatible display.";
		case VK_ERROR_INVALID_SHADER_NV: return "Vulkan invalid shader.";
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
			return "Vulkan invalid DRM format modifier plane layout.";
		case VK_ERROR_NOT_PERMITTED_EXT: return "Vulkan not permitted.";
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			return "Vulkan fullscreen exclusive mode lost.";
		case VK_THREAD_IDLE_KHR: return "Vulkan thread idle.";
		case VK_THREAD_DONE_KHR: return "Vulkan thread done.";
		case VK_OPERATION_DEFERRED_KHR: return "Vulkan operation deferred.";
		case VK_OPERATION_NOT_DEFERRED_KHR: return "Vulkan operation not deferred.";
		case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "Vulkan pipeline compile required.";
		}

		return "Unknown Vulkan result code.";
	}

	std::string String::ConvertUtf8(std::wstring_view string)
	{
		return g_UtfConverter.to_bytes(string.data(), string.data() + string.length());
	}

	char String::ConvertUtf8(wchar_t ch) { return g_UtfConverter.to_bytes(ch)[0]; }

	std::wstring String::ConvertUtf16(std::string_view string)
	{
		return g_UtfConverter.from_bytes(string.data(), string.data() + string.length());
	}

	wchar_t String::ConvertUtf16(char ch) { return g_UtfConverter.from_bytes(ch)[0]; }

	bool String::StartsWith(std::string_view str, std::string_view token)
	{
		if (token.length() > str.length())
			return false;

		return str.compare(0, token.length(), token) == 0;
	}

	bool String::EndsWith(std::string_view str, std::string_view token)
	{
		if (token.length() > str.length())
			return false;

		return str.compare(str.length() - token.length(), token.length(), token) == 0;
	}

	bool String::Contains(std::string_view str, std::string_view token)
	{
		return str.find(token) != std::string::npos;
	}

}  // namespace At0::VulkanTesting
