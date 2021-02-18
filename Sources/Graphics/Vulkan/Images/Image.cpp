#include "pch.h"
#include "Image.h"

#include "Graphics/Graphics.h"
#include "Graphics/Vulkan/LogicalDevice.h"
#include "Graphics/Vulkan/PhysicalDevice.h"
#include "Graphics/Vulkan/Commands/CommandPool.h"
#include "Graphics/Vulkan/Commands/CommandBuffer.h"
#include "../Buffer.h"
#include "../Textures/Sampler.h"
#include "ImageView.h"

#ifdef __GNUC__
	#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image/stb_image.h>


namespace At0::VulkanTesting
{
	Image::Image(const std::string_view filepath)
	{
		int imgWidth, imgHeight, imgChannels;
		stbi_uc* pixels =
			stbi_load(filepath.data(), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

		VkDeviceSize imageSize = imgWidth * imgHeight * 4;

		if (!pixels)
			RAY_THROW_RUNTIME("Failed to load image with path: {0}", filepath);

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Buffer::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(
			Graphics::Get().GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, (size_t)imageSize);
		vkUnmapMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory);

		stbi_image_free(pixels);

		CreateImage(imgWidth, imgHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer, m_Image, imgWidth, imgHeight);
		TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Graphics::Get().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), stagingBufferMemory, nullptr);

		m_ImageView = std::make_unique<ImageView>(
			m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		m_Sampler = std::make_unique<Sampler>();
	}

	Image::~Image()
	{
		vkDestroyImage(Graphics::Get().GetLogicalDevice(), m_Image, nullptr);
		vkFreeMemory(Graphics::Get().GetLogicalDevice(), m_ImageMemory, nullptr);
	}

	void Image::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory)
	{
		VkImageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = format;	 // The format stb_image converts it to
		createInfo.extent = { width, height, 1 };
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = tiling;
		createInfo.usage = usage;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		// createInfo.queueFamilyIndexCount;
		// createInfo.pQueueFamilyIndices;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		RAY_VK_THROW_FAILED(
			vkCreateImage(Graphics::Get().GetLogicalDevice(), &createInfo, nullptr, &image),
			"Failed to create image.");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Graphics::Get().GetLogicalDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex =
			Buffer::FindMemoryType(memRequirements.memoryTypeBits, properties);

		RAY_VK_THROW_FAILED(
			vkAllocateMemory(Graphics::Get().GetLogicalDevice(), &allocInfo, nullptr, &imageMemory),
			"Failed to allocate image memory");
		vkBindImageMemory(Graphics::Get().GetLogicalDevice(), image, imageMemory, 0);
	}

	void Image::TransitionImageLayout(
		VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		CommandBuffer commandBuffer;
		commandBuffer.Begin();

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destStage;
		VkImageMemoryBarrier barrier{};

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			// Has stencil component?
			if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
				 newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
				 newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
									VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
			RAY_THROW_RUNTIME("Unsuported image layout transition");

		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(
			commandBuffer, sourceStage, destStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		commandBuffer.End();
		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		vkQueueSubmit(
			Graphics::Get().GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetLogicalDevice().GetGraphicsQueue());
	}

	void Image::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		CommandBuffer commandBuffer;
		commandBuffer.Begin();

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { width, height, 1 };

		vkCmdCopyBufferToImage(
			commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		commandBuffer.End();
		VkCommandBuffer cmdBuff = commandBuffer;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuff;

		vkQueueSubmit(
			Graphics::Get().GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Graphics::Get().GetLogicalDevice().GetGraphicsQueue());
	}

	VkDescriptorSetLayoutBinding Image::GetDescriptorSetLayout(
		uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlag, uint32_t count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = type;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = stageFlag;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}

	WriteDescriptorSet Image::GetWriteDescriptor(
		uint32_t binding, VkDescriptorType descriptorType) const
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.sampler = *m_Sampler;
		imageInfo.imageView = *m_ImageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = VK_NULL_HANDLE;  // Will be set in the descriptor handler.
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descriptorType;
		descriptorWrite.pImageInfo = &imageInfo;
		return { descriptorWrite, imageInfo };
	}

	std::vector<VkFormat> Image::FindSupportedFormats(
		std::vector<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (int32_t i = candidates.size() - 1; i >= 0; --i)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(
				Graphics::Get().GetPhysicalDevice(), candidates[i], &props);

			if (tiling == VK_IMAGE_TILING_LINEAR &&
				(props.linearTilingFeatures & features) != features)
				candidates.erase(candidates.begin() + i);
			else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
					 (props.optimalTilingFeatures & features) != features)
				candidates.erase(candidates.begin() + i);
		}

		RAY_MEXPECTS(!candidates.empty(), "Unable to find supported Image format");
		return candidates;
	}

}  // namespace At0::VulkanTesting
