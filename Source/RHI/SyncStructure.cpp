#include "RHI/SyncStructure.h"

#include "RHI/CommonHelper.h"
#include "volk.h"

VkSemaphore gran::SyncRHI::CreateSemaphore(VkDevice device, VkSemaphoreCreateFlags flags)
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;

	VkSemaphore semaphore = 0;
	VK_CHECK(vkCreateSemaphore(device, &info, 0, &semaphore));

	return semaphore;
}

VkFence gran::SyncRHI::CreateFence(VkDevice device, VkFenceCreateFlags flags)
{
	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;

	VkFence fence = 0;
	VK_CHECK(vkCreateFence(device, &info, 0, &fence));

	return fence;
}
