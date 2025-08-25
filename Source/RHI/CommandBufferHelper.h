namespace gran
{
	struct Device;
	struct DeviceQueue;
	struct QueueData;
	struct SyncData;
	struct GpuCommandPool;
	struct GpuCommandBuffer;
	struct GpuBuffer;
} // namespace gran

namespace gran::RHI::CommandBuffer
{
	// TODO: Need batching later, for now ok for prototyping
	// Use WriteToGpuBuffer first before uploading the source buffer
	void UploadGpuBufferCmd(
	    const gran::GpuCommandPool& commandPool,
	    const gran::GpuCommandBuffer& cmd,
	    const gran::Device& device,
	    const gran::QueueData queue,
	    const GpuBuffer& srcBuffer,
	    const GpuBuffer& destBuffer);
} // namespace gran::RHI::CommandBuffer
