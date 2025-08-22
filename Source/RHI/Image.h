#pragma once

#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"

// Forward declaration
namespace gran
{
	struct Device;
	struct GpuAllocator;
} // namespace gran

namespace gran
{
	struct GpuImageViewCreateInfo
	{
		VkImageViewCreateInfo m_ImageViewCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.subresourceRange {
			    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			    .baseMipLevel = 0,
			    .levelCount = 1,
			    .baseArrayLayer = 0,
			    .layerCount = 1,
			},
		};
	};

	struct GpuImageCreateInfo
	{
		VkImageCreateInfo m_ImageCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			//.extent = extent,
			.mipLevels = 1,
			.arrayLayers = 1,

			// for MSAA. we will not be using it by default, so default it to 1 sample per pixel.
			.samples = VK_SAMPLE_COUNT_1_BIT,

			// optimal tiling, which means the image is stored on the best gpu format
			// We will hardcode the image tiling to OPTIMAL, which means that we allow the
			// gpu to shuffle the data however it sees fit. If we want to read the image data
			// from cpu, we would need to use tiling LINEAR, which makes the gpu data into a
			// simple 2d array. This tiling highly limits what the gpu can do, so the only
			// real use case for LINEAR is CPU readback.
			.tiling = VK_IMAGE_TILING_OPTIMAL,

			.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,

			//.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		};

		VmaAllocationCreateInfo m_AllocCreateInfo {
			// VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT - if dedicated block is needed like fullscreen image
			// VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT - Can reuse the same memory for non-conficting data
			// Either one is needed for VMA_MEMORY_USAGE_AUTO vmaMapMemory or VMA_ALLOCATION_CREATE_MAPPED_BIT
			// Ensure VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is there
			// VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT - for memcpy all at once, not iterative write/read
			// VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT - can be used for iterative write/read
			.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,

			// Look at typedef enum VmaMemoryUsage
			// VMA_MEMORY_USAGE_AUTO - depends on alloc flag
			// VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE - CPU memory
			// VMA_MEMORY_USAGE_AUTO_PREFER_HOST - GPU memory
			// Block size allocation strategy
			// VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT
			// VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT
			//.usage = VMA_MEMORY_USAGE_GPU_ONLY,
			.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
			// always allocate images on dedicated GPU memory
			.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		};

		bool m_Mipmapped = false;
		void* m_Data;
		bool m_IsDepthImage = false;
	};

	struct GpuImage
	{
		VkImage m_Image;
		VkImageView m_ImageView;
		VkFormat m_ImageFormat;
		VkExtent3D m_ImageExtent;

		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocInfo;
	};

	// Usage: VkImageMemoryBarrier2, ImageView creation, CommandBuffer operations
	// Can also just fill up the subresourceRange directly
	struct GpuImageSubView
	{
		VkImageSubresourceRange subImage {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = VK_REMAINING_MIP_LEVELS,
			.baseArrayLayer = 0,
			.layerCount = VK_REMAINING_ARRAY_LAYERS,
		};
	};

} // namespace gran

namespace gran::RHI::Image
{
	gran::GpuImage CreateGpuImage(const gran::Device& device, const gran::GpuAllocator& allocator, const gran::GpuImageCreateInfo& createInfo);

	void DestroyGpuImage(const gran::Device& device, const gran::GpuAllocator& allocator, const gran::GpuImage& image);

} // namespace gran::RHI::Image
