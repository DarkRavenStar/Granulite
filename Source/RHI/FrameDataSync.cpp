#include "RHI/FrameDataSync.h"

#include "RHI/Device.h"
#include "RHI/SyncStructure.h"
#include "volk.h"

void gran::RHI::Sync::CreateSyncData(
    const gran::DeviceCreationData& creationData,
    const gran::Device& device,
    gran::SyncData& sync)
{
	/*
	The m_SwapchainSemaphore is going to be used so that our render commands wait on the swapchain image request.
	The m_RenderSemaphore will be used to control presenting the image to the OS once the drawing finishes
	The m_RenderFence will lets us wait for the draw commands of a given frame to be finished.
	*/
	// create syncronization structures
	// one fence to control when the gpu has finished rendering the frame,
	// and 2 semaphores to syncronize rendering with swapchain
	// we want the fence to start signalled so we can wait on it on the first frame

	sync.m_ImmediateSubmitFence = gran::RHI::Sync::CreateFence(device.m_Device, VK_FENCE_CREATE_SIGNALED_BIT);
	sync.m_FrameSyncData.resize(creationData.m_MaxFramesInFlight);

	for (uint32_t i = 0; i < creationData.m_MaxFramesInFlight; i++)
	{
		FrameSyncData& frameSync = sync.m_FrameSyncData[i];
		frameSync.m_RenderFence = gran::RHI::Sync::CreateFence(device.m_Device, VK_FENCE_CREATE_SIGNALED_BIT);
		frameSync.m_SwapchainSemaphore = gran::RHI::Sync::CreateSemaphore(device.m_Device);
		frameSync.m_RenderSemaphore = gran::RHI::Sync::CreateSemaphore(device.m_Device);
	}
}

void gran::RHI::Sync::CleanupSyncData(const gran::Device& device, gran::SyncData& sync)
{
	if (device.m_Device == VK_NULL_HANDLE)
	{
		return;
	}

	int maxFrames = sync.m_FrameSyncData.size();

	// Not sure if need to destroy in reverse here
	for (int i = 0; i < maxFrames; i++)
	{
		FrameSyncData& frameSync = sync.m_FrameSyncData[i];
		vkDestroyFence(device.m_Device, frameSync.m_RenderFence, nullptr);
		vkDestroySemaphore(device.m_Device, frameSync.m_SwapchainSemaphore, nullptr);
		vkDestroySemaphore(device.m_Device, frameSync.m_RenderSemaphore, nullptr);
	}

	sync.m_FrameSyncData.clear();

	vkDestroyFence(device.m_Device, sync.m_ImmediateSubmitFence, nullptr);
}