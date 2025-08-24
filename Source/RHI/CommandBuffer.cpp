#include "RHI/CommandBuffer.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/FrameDataSync.h"
#include "volk.h"
#include <functional>

gran::GpuSubmitInfo gran::RHI::CommandBuffer::CreateGpuSubmitInfo(
    const GpuCommandBufferSubmitInfo& cmdSubmitInfo,
    const GpuSemaphoreSubmitInfo* signalSemaphoreInfo,
    const GpuSemaphoreSubmitInfo* waitSemaphoreInfo)
{
	GpuSubmitInfo info;
	VkSubmitInfo2& submitInfo = info.m_SubmitInfo;

	if (signalSemaphoreInfo)
	{
		submitInfo.signalSemaphoreInfoCount = 1;
		submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo->m_SubmitInfo;
	}

	if (waitSemaphoreInfo)
	{
		submitInfo.waitSemaphoreInfoCount = 1;
		submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo->m_SubmitInfo;
	}
	
	submitInfo.commandBufferInfoCount = 1;
	submitInfo.pCommandBufferInfos = &cmdSubmitInfo.m_CmdSubmitInfo;
	
	return info;
}

void gran::RHI::CommandBuffer::ImmediateSubmitCmd(
    const gran::Device& device,
    const gran::DeviceQueue& deviceQueue,
    const gran::SyncData& syncData,
    const GpuCommandBuffer& cmd,
    std::function<void(const GpuCommandBuffer& cmd)>&& function)
{
	VK_CHECK(vkResetFences(device.m_Device, 1, &syncData.m_ImmediateSubmitFence));
	VK_CHECK(vkResetCommandBuffer(cmd.m_Cmd, 0));

	VK_CHECK(vkBeginCommandBuffer(cmd.m_Cmd, &gran::c_DefaultGpuCmdBeginInfo.m_CmdBeginInfo));

	function(cmd);

	VK_CHECK(vkEndCommandBuffer(cmd.m_Cmd));

	GpuCommandBufferSubmitInfo cmdSubmitInfo;
	cmdSubmitInfo.m_CmdSubmitInfo.commandBuffer = cmd.m_Cmd;

	GpuSubmitInfo submit = CreateGpuSubmitInfo(cmdSubmitInfo, nullptr, nullptr);

	VK_CHECK(vkQueueSubmit2(
	    deviceQueue.m_Queue[QueueType::graphics].m_QueueHandle,
	    1,
	    &submit.m_SubmitInfo,
	    syncData.m_ImmediateSubmitFence));

	// Wait for submit to be done
	// VK_CHECK(vkWaitForFences(device.m_Device, 1, &syncData.m_ImmediateSubmitFence, true, 10'000'000'000));
	VK_CHECK(vkWaitForFences(device.m_Device, 1, &syncData.m_ImmediateSubmitFence, true, 9999999999));
}