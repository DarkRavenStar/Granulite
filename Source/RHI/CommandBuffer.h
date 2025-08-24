#include "vulkan/vulkan.h"

// Forward declaration
namespace gran
{
	struct Device;
	struct DeviceQueue;
	struct SyncData;
} // namespace gran

namespace std
{
	template <class>
	class function; // forward declaration
}

namespace gran
{
	struct GpuCommandBufferBeginInfo
	{
		VkCommandBufferBeginInfo m_CmdBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			.pInheritanceInfo = nullptr,
		};
	};

	struct GpuCommandBufferSubmitInfo
	{
		VkCommandBufferSubmitInfo m_CmdSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
			.pNext = nullptr,
			//.commandBuffer,
			.deviceMask = 0,
		};
	};

	inline constexpr GpuCommandBufferBeginInfo c_DefaultGpuCmdBeginInfo {};

	struct GpuCommandBuffer
	{
		VkCommandBuffer m_Cmd;
	};

	struct GpuSemaphoreSubmitInfo
	{
		VkSemaphoreSubmitInfo m_SubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			.pNext = nullptr,
			//.semaphore
			//.stageMask
			.value = 1,
			.deviceIndex = 0,
		};
	};

	struct GpuSubmitInfo
	{
		VkSubmitInfo2 m_SubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
			.pNext = nullptr,
			.waitSemaphoreInfoCount = 0,
			.pWaitSemaphoreInfos = nullptr,
			.commandBufferInfoCount = 0,
			.pCommandBufferInfos = nullptr,
			.signalSemaphoreInfoCount = 0,
			.pSignalSemaphoreInfos = nullptr,
		};
	};

	struct GpuCommandBufferAllocateInfo
	{
		VkCommandBufferAllocateInfo m_CmdAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			//.commandPool,
			//.commandBufferCount,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		};
	};
} // namespace gran

namespace gran::RHI::CommandBuffer
{
	gran::GpuSubmitInfo CreateGpuSubmitInfo(
	    const GpuCommandBufferSubmitInfo& cmdSubmitInfo,
	    const GpuSemaphoreSubmitInfo* signalSemaphoreInfo,
	    const GpuSemaphoreSubmitInfo* waitSemaphoreInfo);

	// Can be terrible inefficient, need batching eventually
	void ImmediateSubmitCmd(
	    const gran::Device& device,
	    const gran::DeviceQueue& deviceQueue,
	    const gran::SyncData& syncData,
	    const GpuCommandBuffer& cmd,
	    std::function<void(const GpuCommandBuffer& cmd)>&& function);

} // namespace gran::RHI::CommandBuffer
