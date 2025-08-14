#include "RHI/Buffer.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Memory.h"
#include "volk.h"

gran::GpuBuffer gran::RHI::Buffer::CreateGpuBuffer(const gran::GpuAllocator& allocator, const gran::GpuBufferCreateInfo& createInfo)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.size = createInfo.m_AllocSize;
	bufferInfo.usage = createInfo.m_BufferUsageFlags;

	VmaAllocationCreateInfo vmaAllocInfo = {};
	vmaAllocInfo.usage = createInfo.m_MemoryUsage;
	vmaAllocInfo.flags = createInfo.m_MemoryAllocTypeFlags;

	GpuBuffer buffer;

	VK_CHECK(vmaCreateBuffer(
	    allocator.m_Allocator,
	    &bufferInfo,
	    &vmaAllocInfo,
	    &buffer.m_Buffer,
	    &buffer.m_Allocation,
	    &buffer.m_AllocInfo));

	return buffer;
}

void gran::RHI::Buffer::DestroyBuffer(const gran::GpuAllocator& allocator, const gran::GpuBuffer& buffer)
{
	vmaDestroyBuffer(allocator.m_Allocator, buffer.m_Buffer, buffer.m_Allocation);
}

VkDeviceAddress gran::RHI::Buffer::GetBufferAddress(const gran::Device& device, const gran::GpuBuffer& buffer)
{
	VkBufferDeviceAddressInfo info = { VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
	info.buffer = buffer.m_Buffer;

	VkDeviceAddress address = vkGetBufferDeviceAddress(device.m_Device, &info);
	assert(address != 0);

	return address;
}
