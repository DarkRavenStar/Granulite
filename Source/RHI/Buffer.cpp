#include "RHI/Buffer.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Memory.h"
#include "volk.h"

namespace
{
	void CheckAndAssertNonMapping(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
	{
		VkMemoryPropertyFlags result;
		vmaGetMemoryTypeProperties(allocator.m_Allocator, buffer.m_AllocInfo.memoryType, &result);

		if (!(result & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
		{
			assert(!"Operation not allowed for non-mapping type");
			return;
		}
	}
} // namespace

gran::GpuBuffer gran::RHI::Buffer::CreateGpuBuffer(const gran::GpuAllocator& allocator, const gran::GpuBufferCreateInfo& createInfo)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.size = createInfo.m_AllocSize;
	bufferInfo.usage = createInfo.m_BufferUsageFlags;
	// If we use same command queue for the compute and graphics pipeline
	// create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// create_info.queueFamilyIndexCount = 1;
	// const uint32_t queue_family_indices[1] = { deviceQueue.m_Queue[QueueType::graphics].m_QueueFamily };
	// create_info.pQueueFamilyIndices = queue_family_indices;

	VmaAllocationCreateInfo vmaAllocInfo = {};
	vmaAllocInfo.usage = createInfo.m_MemoryUsage;
	vmaAllocInfo.flags = createInfo.m_MemoryAllocFlags;
	// vmaAllocInfo.priority = 1.0f;
	// vmaAllocInfo.pool;
	// vmaAllocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	// vmaAllocInfo.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	// vmaAllocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	// vmaAllocInfo.preferredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

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

void gran::RHI::Buffer::DestroyGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
{
	vmaDestroyBuffer(allocator.m_Allocator, buffer.m_Buffer, buffer.m_Allocation);
	buffer = {};
}

void* gran::RHI::Buffer::MapGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
{
	CheckAndAssertNonMapping(allocator, buffer);

	VK_CHECK(vmaMapMemory(allocator.m_Allocator, buffer.m_Allocation, &buffer.m_AllocInfo.pMappedData));

	return buffer.m_AllocInfo.pMappedData;
}

void gran::RHI::Buffer::UnmapGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
{
	CheckAndAssertNonMapping(allocator, buffer);

	vmaUnmapMemory(allocator.m_Allocator, buffer.m_Allocation);
}

void gran::RHI::Buffer::WriteToGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer, void* data, uint64_t sizeInBytes)
{
	CheckAndAssertNonMapping(allocator, buffer);

	void* mappedMemory = MapGpuBuffer(allocator, buffer);

	assert(mappedMemory);
	assert(data);

	if (buffer.m_AllocInfo.size < sizeInBytes)
	{
		return;
	}

	memcpy(mappedMemory, data, sizeInBytes);

	gran::RHI::Buffer::UnmapGpuBuffer(allocator, buffer);

	gran::RHI::Buffer::FlushGpuBuffer(allocator, buffer);

	// allocator.m_Allocator->CopyMemoryToAllocation(data, buffer.m_Allocation, 0, sizeInBytes)
}

void gran::RHI::Buffer::CopyFromGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer, void* data, uint64_t sizeInBytes)
{
	CheckAndAssertNonMapping(allocator, buffer);

	void* mappedMemory = MapGpuBuffer(allocator, buffer);

	assert(mappedMemory);
	assert(data);

	if (buffer.m_AllocInfo.size < sizeInBytes)
	{
		return;
	}

	gran::RHI::Buffer::InvalidateGpuBuffer(allocator, buffer);

	memcpy(data, mappedMemory, sizeInBytes);

	gran::RHI::Buffer::UnmapGpuBuffer(allocator, buffer);

	// allocator.m_Allocator->CopyAllocationToMemory(buffer.m_Allocation, 0, data, sizeInBytes)
}

// For CPU to GPU data sync
void gran::RHI::Buffer::FlushGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
{
	CheckAndAssertNonMapping(allocator, buffer);

	vmaFlushAllocation(allocator.m_Allocator, buffer.m_Allocation, 0, VK_WHOLE_SIZE);
}

// For GPU to CPU data sync
void gran::RHI::Buffer::InvalidateGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer)
{
	CheckAndAssertNonMapping(allocator, buffer);

	vmaInvalidateAllocation(allocator.m_Allocator, buffer.m_Allocation, 0, VK_WHOLE_SIZE);
}

VkDeviceAddress gran::RHI::Buffer::GetBufferAddress(const gran::Device& device, const gran::GpuBuffer& buffer)
{
	VkBufferDeviceAddressInfo info {};
	info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	info.buffer = buffer.m_Buffer;

	VkDeviceAddress address = vkGetBufferDeviceAddress(device.m_Device, &info);
	assert(address != 0);

	return address;
}
