#pragma once

#include "pch.h"

#include <memory>
#include <array>

#include "Camera.h"

#include "Utils/RAssert.h"
#include "Utils/RException.h"


namespace At0::VulkanTesting
{
	class VulkanInstance;
	class PhysicalDevice;
	class LogicalDevice;
	class Surface;
	class Swapchain;
	class CommandPool;
	class CommandBuffer;
	class Renderpass;
	class Framebuffer;
	class DepthImage;
	class Drawable;

	class Model;

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
		void RecordCommandBuffer(CommandBuffer& cmdBuff, const Framebuffer& framebuffer);
		void CreateSyncObjects();
		void RecreateSwapchain();

		void UpdateViewport();
		void UpdateScissor();
		void UpdateDrawables();

	private:
		// Utils
		float m_Dt;
		bool m_FramebufferResized = false;

		// Specifies the maximum amount of images which we can work on concurrently
		// (1 image is presenting, the other 2 of triple buffering can be worked on)
		static constexpr uint8_t s_MaxFramesInFlight = 2;
		uint64_t m_CurrentFrame = 0;
		inline static Graphics* s_Instance = nullptr;

		// Extensions functions

		// Dynamic state, need to be updated when the window is resized
		VkViewport m_Viewport;
		VkRect2D m_Scissor;

		Scope<VulkanInstance> m_Instance;
		Scope<PhysicalDevice> m_PhysicalDevice;
		Scope<Surface> m_Surface;
		Scope<LogicalDevice> m_LogicalDevice;
		Scope<Swapchain> m_Swapchain;
		Scope<Renderpass> m_Renderpass;
		Scope<DepthImage> m_DepthImage;
		Scope<CommandPool> m_CommandPool;
		std::vector<Scope<CommandBuffer>> m_CommandBuffers;

		std::vector<Scope<Framebuffer>> m_Framebuffers;

		// Signal that an image has been acquired and is ready for rendering (1 semaphore / frame)
		std::array<VkSemaphore, s_MaxFramesInFlight> m_ImageAvailableSemaphore;

		// Signal that rendering is finished and ready for presentation (1 semaphore / frame)
		std::array<VkSemaphore, s_MaxFramesInFlight> m_RenderFinishedSemaphore;

		// We may be using the frame #0 objects while frame #0 is still in-flight
		std::array<VkFence, s_MaxFramesInFlight> m_InFlightFences;

		// We need to track for each swap chain image if a frame in flight is currently using it
		std::vector<VkFence> m_ImagesInFlight;

		std::vector<Scope<Drawable>> m_Drawables;
		Scope<Model> m_Model;
		Scope<Model> m_Model2;
	};
}  // namespace At0::VulkanTesting
