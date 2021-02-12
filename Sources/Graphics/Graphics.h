#pragma once

#include <memory>
#include <array>

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
#include "Bindables/Pipeline.h"
#include "Renderpass/Renderpass.h"
#include "Framebuffer.h"
#include "Drawables/Triangle.h"


namespace At0::VulkanTesting
{
	class Graphics
	{
		friend std::unique_ptr<Graphics> std::make_unique<Graphics>();
		friend class Window;

	public:
		~Graphics();
		static Graphics& Get();
		static void Create();

		void Update();

		const VulkanInstance& GetInstance() const { return *m_Instance; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice; }
		const Surface& GetSurface() const { return *m_Surface; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }
		const Renderpass& GetRenderpass() const { return *m_Renderpass; }

	private:
		Graphics();
		void CreateRenderpass();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandBuffers();
		void RecordCommandBuffer(
			std::unique_ptr<CommandBuffer>& cmdBuff, std::unique_ptr<Framebuffer>& framebuffer);
		void CreateSyncObjects();
		void RecreateSwapchain();

		void UpdateViewport();
		void UpdateScissor();

	private:
		// Specifies the maximum amount of images which we can work on concurrently
		// (1 image is presenting, the other 2 of triple buffering can be worked on)
		static constexpr uint8_t s_MaxFramesInFlight = 2;
		uint64_t m_CurrentFrame = 0;
		inline static Graphics* s_Instance = nullptr;

		// Dynamic state, need to be updated when the window is resized
		VkViewport m_Viewport;
		VkRect2D m_Scissor;

		std::unique_ptr<VulkanInstance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<Swapchain> m_Swapchain;
		std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
		std::unique_ptr<Renderpass> m_Renderpass;
		std::unique_ptr<CommandPool> m_CommandPool;
		std::vector<std::unique_ptr<CommandBuffer>> m_CommandBuffers;

		std::vector<std::unique_ptr<Framebuffer>> m_Framebuffers;

		// Signal that an image has been acquired and is ready for rendering (1 semaphore / frame)
		std::array<VkSemaphore, s_MaxFramesInFlight> m_ImageAvailableSemaphore;

		// Signal that rendering is finished and ready for presentation (1 semaphore / frame)
		std::array<VkSemaphore, s_MaxFramesInFlight> m_RenderFinishedSemaphore;

		// We may be using the frame #0 objects while frame #0 is still in-flight
		std::array<VkFence, s_MaxFramesInFlight> m_InFlightFences;

		// We need to track for each swap chain image if a frame in flight is currently using it
		std::vector<VkFence> m_ImagesInFlight;

		bool m_FramebufferResized = false;

		std::unique_ptr<Triangle> m_Triangle;
	};
}  // namespace At0::VulkanTesting
