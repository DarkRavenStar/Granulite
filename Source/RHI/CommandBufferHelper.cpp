#include "RHI/CommandBufferHelper.h"

#include "RHI/Buffer.h"
#include "RHI/CommonHelper.h"
#include "RHI/CommandBuffer.h"
#include "RHI/CommandPool.h"
#include "RHI/Device.h"
#include "volk.h"

void gran::RHI::CommandBuffer::UploadGpuBufferCmd(
    const gran::GpuCommandPool& commandPool,
    const gran::GpuCommandBuffer& cmd,
    const gran::Device& device,
    const gran::QueueData queue,
    const GpuBuffer& srcBuffer,
    const GpuBuffer& destBuffer)
{
	// TODO: Check the size between 2 buffer compatibility
	gran::RHI::CommandPool::ResetGpuCommandPool(device, commandPool);
	gran::RHI::CommandBuffer::BeginGpuCommandBuffer(cmd);

	VkBufferCopy region = { 0, 0, destBuffer.m_AllocInfo.size };
	vkCmdCopyBuffer(cmd.m_Cmd, srcBuffer.m_Buffer, destBuffer.m_Buffer, 1, &region);

	gran::RHI::CommandBuffer::EndGpuCommandBuffer(cmd);

	gran::GpuCommandBufferSubmitInfo cmdSubmitInfo;
	cmdSubmitInfo.m_CmdSubmitInfo.commandBuffer = cmd.m_Cmd;

	gran::GpuSubmitInfo submit = gran::RHI::CommandBuffer::CreateGpuSubmitInfo(cmdSubmitInfo, nullptr, nullptr);

	VK_CHECK(vkQueueSubmit2(
	    queue.m_QueueHandle,
	    1,
	    &submit.m_SubmitInfo,
	    VK_NULL_HANDLE));

	VK_CHECK(vkDeviceWaitIdle(device.m_Device));
};