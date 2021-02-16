#pragma once

#include <memory>
#include <array>

#include "Utils/RAssert.h"
#include "Utils/RException.h"

#include "Vulkan/Commands/CommandBuffer.h"
#include "Vulkan/Commands/CommandPool.h"
#include "Vulkan/Framebuffer.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/Surface.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/Swapchain.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Renderpass/Renderpass.h"
#include "Vulkan/Framebuffer.h"
#include "Vulkan/Descriptor.h"
#include "Vulkan/UniformBuffer.h"

#include "Camera.h"

#include "Primitives/Triangle.h"
#include "Primitives/Square.h"
#include "Primitives/Cube.h"


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
		static void Destroy();

		void Update(float dt);

		float GetDelta() const { return m_Dt; }

		const VulkanInstance& GetInstance() const { return *m_Instance; }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice; }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice; }
		const Surface& GetSurface() const { return *m_Surface; }
		const Swapchain& GetSwapchain() const { return *m_Swapchain; }
		const CommandPool& GetCommandPool() const { return *m_CommandPool; }
		const Renderpass& GetRenderpass() const { return *m_Renderpass; }

	public:
		Camera SceneCamera;

	private:
		Graphics();
		void LoadExtensionFunctions();
		void CreateRenderpass();
		void CreateFramebuffers();
		void CreateDrawables();
		void CreateCommandBuffers();
		void RecordCommandBuffer(std::unique_ptr<CommandBuffer>& cmdBuff,
			std::unique_ptr<Framebuffer>& framebuffer,
			std::unique_ptr<DescriptorSet>& descriptorSet);
		void CreateSyncObjects();
		void RecreateSwapchain();

		void UpdateViewport();
		void UpdateScissor();

		// Temporary
		void CreateDescriptorSetLayout();
		void CreateUniformBuffers();
		void UpdateUniformBuffer(uint32_t currentImage);
		void CreateDescriptorPool();
		void CreateDescriptorSets();

	private:
		// Utils
		float m_Dt;

		// Specifies the maximum amount of images which we can work on concurrently
		// (1 image is presenting, the other 2 of triple buffering can be worked on)
		static constexpr uint8_t s_MaxFramesInFlight = 2;
		uint64_t m_CurrentFrame = 0;
		inline static Graphics* s_Instance = nullptr;

		// Extensions functions

		// Dynamic state, need to be updated when the window is resized
		VkViewport m_Viewport;
		VkRect2D m_Scissor;

		std::unique_ptr<VulkanInstance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<Swapchain> m_Swapchain;
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

		std::unique_ptr<Drawable> m_Drawable;

		// Uniform buffer stuff
		// std::unique_ptr<DescriptorSetLayout> m_DescriptorSetLayout;

		// Temporary data
		std::vector<std::unique_ptr<UniformBuffer>> m_UniformBuffers;
		// std::unique_ptr<DescriptorPool> m_DescriptorPool;
		std::vector<std::unique_ptr<DescriptorSet>> m_DescriptorSets;
	};
}  // namespace At0::VulkanTesting
