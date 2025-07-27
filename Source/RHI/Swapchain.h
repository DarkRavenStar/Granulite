#pragma once

#include "vulkan/vulkan.h"
#include <vector>

// Forward Declaration
namespace gran
{
	struct DeviceCreationData;
	struct Device;
	struct DeviceQueue;
} // namespace gran

namespace gran
{
	struct Swapchain
	{
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		VkFormat m_SwapchainImageFormat = VK_FORMAT_UNDEFINED;
		VkColorSpaceKHR m_ColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		VkPresentModeKHR m_PresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		VkExtent2D m_SwapchainExtent = { 0, 0 };
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;
	};
} // namespace gran

namespace gran::RHI
{
	void CreateSwapchain(
	    const gran::DeviceCreationData& deviceCreationData,
	    const gran::Device& device,
	    const gran::DeviceQueue& deviceQueue,
	    const VkSwapchainKHR& oldSwapchain,
	    const VkExtent2D& newSwapchainSize,
	    gran::Swapchain& swapchain);

	void DestroySwapchain(
	    const VkDevice& device,
	    const VkSwapchainKHR& swapchain,
	    const std::vector<VkImageView>& swapchainImageViews);

} // namespace gran::RHI
