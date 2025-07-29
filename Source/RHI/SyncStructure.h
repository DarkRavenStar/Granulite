#pragma once
#include "vulkan/vulkan.h"

namespace gran::SyncRHI
{
	VkSemaphore CreateSemaphore(VkDevice device, VkSemaphoreCreateFlags flags = 0);

	VkFence CreateFence(VkDevice device, VkFenceCreateFlags flags = 0);

} // namespace gran::SyncRHI
