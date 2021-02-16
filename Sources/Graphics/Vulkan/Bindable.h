#pragma once


namespace At0::VulkanTesting
{
	class CommandBuffer;

	class Bindable
	{
	public:
		virtual ~Bindable() = default;

		virtual void Bind(const CommandBuffer& cmdBuff) = 0;
	};
}  // namespace At0::VulkanTesting
