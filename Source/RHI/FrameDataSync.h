
#pragma once
#include "vulkan/vulkan.h"
#include <vector>

namespace gran
{
	struct DeviceCreationData;
	struct Device;
} // namespace gran

namespace gran
{
	struct FrameSyncData
	{
		// Used to check if the GPU has finished rendering
		// Will cause syncing when reusing the same index
		// when GPU is not done yet
		VkFence m_RenderFence;

		// Use to sync when getting image from swapchain
		// and when the swapchain will be ready to be used
		VkSemaphore m_SwapchainSemaphore;

		// Use to sync when the rendering is done and
		// can we transfer the data to the swapchain
		VkSemaphore m_RenderSemaphore;
	};

	struct SyncData
	{
		// Used for submitting any immediate stuff
		VkFence m_ImmediateSubmitFence;
		std::vector<gran::FrameSyncData> m_FrameSyncData;
	};

} // namespace gran

namespace gran::RHI::Sync
{
	void CreateSyncData(
	    const gran::DeviceCreationData& creationData,
	    const gran::Device& device,
	    gran::SyncData& sync);

	void CleanupSyncData(const gran::Device& device, gran::SyncData& sync);

} // namespace gran::RHI::Sync
