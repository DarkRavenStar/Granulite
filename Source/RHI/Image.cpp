#include "RHI/Image.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Memory.h"
#include "volk.h"
#include <algorithm>
#include <cmath>

gran::GpuImage gran::RHI::Image::CreateGpuImage(const gran::Device& device, const gran::GpuAllocator& allocator, const gran::GpuImageCreateInfo& createInfo)
{
	VkImageCreateInfo imageCreateInfo = createInfo.m_ImageCreateInfo;

	GpuImage newImage;
	newImage.m_ImageFormat = imageCreateInfo.format;
	newImage.m_ImageExtent = imageCreateInfo.extent;

	if (createInfo.m_Mipmapped)
	{
		VkExtent3D size = imageCreateInfo.extent;
		double baseMipLevel = std::floor(std::log2(std::max<uint32_t>(size.width, size.height)));
		imageCreateInfo.mipLevels = static_cast<uint32_t>(baseMipLevel) + 1;
	}

	VK_CHECK(vmaCreateImage(
	    allocator.m_Allocator,
	    &createInfo.m_ImageCreateInfo,
	    &createInfo.m_AllocCreateInfo,
	    &newImage.m_Image,
	    &newImage.m_Allocation,
	    &newImage.m_AllocInfo));

	VkImageAspectFlags aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;
	if (imageCreateInfo.format == VK_FORMAT_D32_SFLOAT) // Usual depth format
	{
		aspectFlag = VK_IMAGE_ASPECT_DEPTH_BIT;
	}

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.format = newImage.m_ImageFormat;
	imageViewCreateInfo.image = newImage.m_Image;
	imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;
	imageViewCreateInfo.subresourceRange.levelCount = imageCreateInfo.mipLevels;

	VK_CHECK(vkCreateImageView(
	    device.m_Device,
	    &imageViewCreateInfo,
	    nullptr,
	    &newImage.m_ImageView));

	return newImage;
}

void gran::RHI::Image::DestroyGpuImage(const gran::Device& device, const gran::GpuAllocator& allocator, const gran::GpuImage& image)
{
	vkDestroyImageView(device.m_Device, image.m_ImageView, nullptr);
	vmaDestroyImage(allocator.m_Allocator, image.m_Image, image.m_Allocation);
}
