#pragma once

#include <memory>
#include "../Utils/RAssert.h"
#include "../Utils/RException.h"

#include "Commands/CommandBuffer.h"
#include "Commands/CommandPool.h"
#include "Framebuffer.h"
#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "Surface.h"
#include "LogicalDevice.h"
#include "Swapchain.h"
#include "Pipeline/Pipeline.h"


namespace At0::VulkanTesting
{
	class Graphics
	{
		friend std::unique_ptr<Graphics> std::make_unique<Graphics>();

	public:
		~Graphics();
		static Graphics& Get();

		const VulkanInstance& GetInstance() const { return *m_Instance; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice; }
		const Surface& GetSurface() const { return *m_Surface; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }

	private:
		Graphics();

	private:
		inline static Graphics* s_Instance = nullptr;

		std::unique_ptr<VulkanInstance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<CommandPool> m_CommandPool;
		std::unique_ptr<Swapchain> m_Swapchain;
		std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
	};
}  // namespace At0::VulkanTesting
