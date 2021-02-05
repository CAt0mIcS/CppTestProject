#pragma once

#include <memory>
#include "../Utils/RAssert.h"


namespace At0::VulkanTesting
{
	class VulkanInstance;
	class PhysicalDevice;
	class LogicalDevice;
	class Surface;
	class CommandPool;

	class Graphics
	{
		friend std::unique_ptr<Graphics> std::make_unique<Graphics>();

	public:
		~Graphics();
		static void Create();
		static Graphics& Get()
		{
			RAY_MEXPECTS(s_Instance, "Instance not created.");
			return *s_Instance;
		}

		const VulkanInstance& GetInstance() const { return *m_Instance; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice; }
		const Surface& GetSurface() const { return *m_Surface; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }

	private:
		Graphics();

	private:
		inline static std::unique_ptr<Graphics> s_Instance = nullptr;

		std::unique_ptr<VulkanInstance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<CommandPool> m_CommandPool;
	};
}  // namespace At0::VulkanTesting
