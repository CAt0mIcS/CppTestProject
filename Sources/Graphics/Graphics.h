#pragma once

#include <memory>


namespace At0::VulkanTesting
{
	class VulkanInstance;
	class PhysicalDevice;
	class LogicalDevice;

	class Graphics
	{
		friend std::unique_ptr<Graphics> std::make_unique<Graphics>();

	public:
		~Graphics();
		static void Create();
		static Graphics& Get() { return *s_Instance; }

		const VulkanInstance& GetInstance() const { return *m_Instance; }

	private:
		Graphics();

	private:
		inline static std::unique_ptr<Graphics> s_Instance = nullptr;

		std::unique_ptr<VulkanInstance> m_Instance;
	};
}  // namespace At0::VulkanTesting
