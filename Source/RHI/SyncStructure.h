#pragma once
#include "vulkan/vulkan.h"

namespace gran::RHI::Sync
{
	VkSemaphore CreateSemaphore(const VkDevice& device, VkSemaphoreCreateFlags flags = 0);

	VkFence CreateFence(const VkDevice& device, VkFenceCreateFlags flags = 0);

} // namespace gran::RHI::Sync
