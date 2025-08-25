#include "RHI/CommandPool.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "volk.h"

gran::GpuCommandPool gran::RHI::CommandPool::CreateGpuCommandPool(
    const gran::Device& device,
    const gran::GpuCommandPoolCreateInfo& createInfo)
{
	VkCommandPool commandPool = 0;
	VK_CHECK(vkCreateCommandPool(
	    device.m_Device,
	    &createInfo.m_CreateInfo,
	    nullptr,
	    &commandPool));

	return { commandPool };
}

void gran::RHI::CommandPool::ResetGpuCommandPool(
    const gran::Device& device,
    const gran::GpuCommandPool& pool)
{
	VK_CHECK(vkResetCommandPool(device.m_Device, pool.m_CommandPool, 0));
}

void gran::RHI::CommandPool::DestroyGpuCommandPool(
    const gran::Device& device,
    const gran::GpuCommandPool& pool)
{
	vkDestroyCommandPool(device.m_Device, pool.m_CommandPool, nullptr);
}