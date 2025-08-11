#include "RHI/FrameDataSync.h"

#include "RHI/Device.h"
#include "RHI/SyncStructure.h"
#include "volk.h"

void gran::RHI::Sync::CreateFrameSyncData(
    const gran::DeviceCreationData& creationData,
    const gran::Device& device,
    gran::FrameSyncData& sync)
{
	// create syncronization structures
	// one fence to control when the gpu has finished rendering the frame,
	// and 2 semaphores to syncronize rendering with swapchain
	// we want the fence to start signalled so we can wait on it on the first frame
	uint32_t maxFrames = creationData.m_MaxFramesInFlight;

	sync.m_ImmediateSubmitFence = gran::RHI::Sync::CreateFence(device.m_Device, VK_FENCE_CREATE_SIGNALED_BIT);

	sync.m_MaxFrames = maxFrames;
	sync.m_RenderFence.resize(maxFrames);
	sync.m_SwapchainSemaphore.resize(maxFrames);
	sync.m_RenderSemaphore.resize(maxFrames);

	for (uint32_t i = 0; i < maxFrames; i++)
	{
		sync.m_RenderFence[i] = gran::RHI::Sync::CreateFence(device.m_Device, VK_FENCE_CREATE_SIGNALED_BIT);
		sync.m_SwapchainSemaphore[i] = gran::RHI::Sync::CreateSemaphore(device.m_Device);
		sync.m_RenderSemaphore[i] = gran::RHI::Sync::CreateSemaphore(device.m_Device);
	}
}

void gran::RHI::Sync::CleanupFrameSyncData(const gran::Device& device, gran::FrameSyncData& sync)
{
	if (device.m_Device == VK_NULL_HANDLE)
	{
		return;
	}

	int maxFrames = sync.m_RenderFence.size();

	// Not sure if need to destroy in reverse here
	for (int i = 0; i < maxFrames; i++)
	{
		vkDestroyFence(device.m_Device, sync.m_RenderFence[i], nullptr);
		vkDestroySemaphore(device.m_Device, sync.m_SwapchainSemaphore[i], nullptr);
		vkDestroySemaphore(device.m_Device, sync.m_RenderSemaphore[i], nullptr);
	}

	sync.m_RenderFence.clear();
	sync.m_SwapchainSemaphore.clear();
	sync.m_RenderSemaphore.clear();

	vkDestroyFence(device.m_Device, sync.m_ImmediateSubmitFence, nullptr);
}