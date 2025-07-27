#include "RHI/Swapchain.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "VkBootstrap.h"
#include "glfw/glfw3.h"
#include "volk.h"

void gran::RHI::CreateSwapchain(
    const gran::DeviceCreationData& deviceCreationData,
    const gran::Device& device,
    const gran::DeviceQueue& deviceQueue,
    const VkSwapchainKHR& oldSwapchain,
    const VkExtent2D& newSwapchainSize,
    gran::Swapchain& swapchain)
{

	vkb::SwapchainBuilder swapchainBuilder {
		device.m_PhysicalDevice,
		device.m_Device,
		device.m_Surface,
		deviceQueue.m_Queue[QueueType::graphics].m_QueueFamily,
		deviceQueue.m_Queue[QueueType::present].m_QueueFamily,
	};

	// If we want to smoother transition from old frame
	swapchainBuilder.set_old_swapchain(oldSwapchain);

	// Added most supported surface image format for Windows for now
	swapchainBuilder.set_desired_format(VkSurfaceFormatKHR { .format = VK_FORMAT_B8G8R8A8_UNORM, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });
	// swapchainBuilder.set_fallback_format(VkSurfaceFormatKHR { .format = VK_FORMAT_R8B8G8A8_UNORM, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR });
	// swapchainBuilder.use_default_format_selection()

	// Mailbox is fine if we don't need vsync - can revisit in the future
	swapchainBuilder.set_desired_present_mode(deviceCreationData.m_UseVSync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR);
	// swapchainBuilder.set_desired_present_mode();
	// swapchainBuilder.use_default_present_mode_selection()

	swapchainBuilder.set_desired_extent(newSwapchainSize.width, newSwapchainSize.height);
	swapchainBuilder.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT);
	// Not sure if we need this????
	// swapchainBuilder.add_image_usage_flags(VK_IMAGE_USAGE_STORAGE_BIT);

	// swapchainBuilder.set_composite_alpha_flags(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	// swapchainBuilder.set_desired_min_image_count(BufferMode::DOUBLE_BUFFERING)

	vkb::Swapchain vkbSwapchain = swapchainBuilder.build().value();

	swapchain.m_SwapchainImageFormat = vkbSwapchain.image_format;
	swapchain.m_Swapchain = vkbSwapchain.swapchain;
	swapchain.m_SwapchainExtent = vkbSwapchain.extent;
	swapchain.m_SwapchainImages = vkbSwapchain.get_images().value();
	swapchain.m_SwapchainImageViews = vkbSwapchain.get_image_views().value();
}

void gran::RHI::DestroySwapchain(
    const VkDevice& device,
    const VkSwapchainKHR& swapchain,
    const std::vector<VkImageView>& swapchainImageViews)
{
	if (device == VK_NULL_HANDLE && swapchain == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < swapchainImageViews.size(); i++)
	{
		vkDestroyImageView(device, swapchainImageViews[i], nullptr);
	}
}
