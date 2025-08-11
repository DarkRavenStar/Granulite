
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
		// Used for submitting any immediate stuff
		VkFence m_ImmediateSubmitFence;

		uint32_t m_MaxFrames = 0;
		// Used to check if the GPU has finished rendering
		// Will cause syncing when reusing the same index
		// when GPU is not done yet
		std::vector<VkFence> m_RenderFence;
		// Use to sync when getting image from swapchain
		// and when the swapchain will be ready to be used
		std::vector<VkSemaphore> m_SwapchainSemaphore;

		// Use to sync when the rendering is done and
		// can we transfer the data to the swapchain
		std::vector<VkSemaphore> m_RenderSemaphore;
	};

} // namespace gran

namespace gran::RHI::Sync
{
	void CreateFrameSyncData(
	    const gran::DeviceCreationData& creationData,
	    const gran::Device& device,
	    gran::FrameSyncData& sync);

	void CleanupFrameSyncData(const gran::Device& device, gran::FrameSyncData& sync);

} // namespace gran::RHI::Sync
