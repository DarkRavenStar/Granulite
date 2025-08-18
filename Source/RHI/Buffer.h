#pragma once
#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"
#include <vector>

// Forward declaration
namespace gran
{
	struct Device;
	struct GpuAllocator;
} // namespace gran

namespace gran
{
	// TODO:
	//  API to create/use "staging" buffer and
	//  issue an explicit transfer to write/read your data.
	//  Need to use flags like
	//  VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
	//  VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT

	// To help with forward declaration
	// we could use raw pointer if needed
	struct GpuBuffer
	{
		VkBuffer m_Buffer;

		VmaAllocation m_Allocation = VK_NULL_HANDLE;
		VmaAllocationInfo m_AllocInfo;
	};

	struct GpuBufferCreateInfo
	{
		VkBufferCreateInfo m_BufferCreateInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			//.size = 1,
			// VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT can be used if device address is supported
			.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		};

		VmaAllocationCreateInfo m_AllocCreateInfo {
			// VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT - if dedicated block is needed like fullscreen image
			// VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT - Can reuse the same memory for non-conficting data
			// Either one is needed for VMA_MEMORY_USAGE_AUTO vmaMapMemory or VMA_ALLOCATION_CREATE_MAPPED_BIT
			// Ensure VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is there
			// VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT - for memcpy all at once, not iterative write/read
			// VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT - can be used for iterative write/read
			.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,

			// Look at typedef enum VmaMemoryUsage
			// VMA_MEMORY_USAGE_AUTO - depends on alloc flag
			// VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE - CPU memory
			// VMA_MEMORY_USAGE_AUTO_PREFER_HOST - GPU memory
			// Block size allocation strategy
			// VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT
			// VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT
			.usage = VMA_MEMORY_USAGE_AUTO,
		};
	};

} // namespace gran

namespace gran::RHI::Buffer
{
	gran::GpuBuffer CreateGpuBuffer(const gran::GpuAllocator& allocator, const gran::GpuBufferCreateInfo& createInfo);

	void DestroyGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer);

	void* MapGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer);

	void UnmapGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer);

	void WriteToGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer, void* data, uint64_t sizeInBytes);

	void CopyFromGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer, void* data, uint64_t sizeInBytes);

	// For CPU to GPU data sync
	void FlushGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer);

	// For GPU to CPU data sync
	void InvalidateGpuBuffer(const gran::GpuAllocator& allocator, gran::GpuBuffer& buffer);

	// Get raw GPU pointer to a buffer and pass it for usage inside a shader or other use cases.
	VkDeviceAddress GetBufferAddress(const gran::Device& device, const gran::GpuBuffer& buffer);

} // namespace gran::RHI::Buffer

/*
// Incomplete and experimental
static gran::GpuBufferCreateInfo StagingGpuBufferCreateInfo()
{
    // It will likely end up in a memory type that is `HOST_VISIBLE` and `HOST_COHERENT`
    // but not `HOST_CACHED` (meaning uncached, write-combined) and not `DEVICE_LOCAL` (meaning system RAM).
    gran::GpuBufferCreateInfo createInfo;
    createInfo.m_BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    createInfo.m_MemoryUsage = VMA_MEMORY_USAGE_AUTO;
    createInfo.m_MemoryAllocFlags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

    return createInfo;
}
*/

static void BufferTesting(const gran::GpuAllocator& allocator)
{
	const VkDeviceSize bufferSize = 65536;
	std::vector<std::uint8_t> bufferData(bufferSize);
	for (auto& bufferByte : bufferData)
	{
		bufferByte = static_cast<std::uint8_t>(rand() % 256);
	}

	// We are creating cpu visible side buffer first
	gran::GpuBufferCreateInfo createInfo {};
	createInfo.m_BufferCreateInfo.size = bufferSize;
	createInfo.m_BufferCreateInfo.usage =
	    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
	    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	    VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

	createInfo.m_AllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
	// createInfo.m_MemoryUsage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
	// createInfo.m_MemoryAllocFlags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
	// createInfo.m_MemoryAllocFlags =
	// VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
	//  VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT|
	//  VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
	// VMA_ALLOCATION_CREATE_MAPPED_BIT;
	// VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

	gran::GpuBuffer buffer = gran::RHI::Buffer::CreateGpuBuffer(allocator, createInfo);

	gran::RHI::Buffer::MapGpuBuffer(allocator, buffer);

	gran::RHI::Buffer::WriteToGpuBuffer(allocator, buffer, bufferData.data(), bufferData.size());

	bufferData.clear();
	bufferData.resize(bufferSize);

	gran::RHI::Buffer::CopyFromGpuBuffer(allocator, buffer, bufferData.data(), bufferData.size());

	gran::RHI::Buffer::UnmapGpuBuffer(allocator, buffer);

	gran::RHI::Buffer::DestroyGpuBuffer(allocator, buffer);
}

/*
static void TransferBufferTesting(const gran::GpuAllocator& allocator)
{
    const VkDeviceSize bufferSize = 65536;
    std::vector<std::uint8_t> bufferData(bufferSize);
    for (auto& bufferByte : bufferData)
    {
        bufferByte = static_cast<std::uint8_t>(rand() % 256);
    }

    gran::GpuBufferCreateInfo createInfo {};
    createInfo.m_AllocSize = 65536;
    createInfo.m_BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    createInfo.m_MemoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    createInfo.m_MemoryAllocFlags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
    // VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

    gran::GpuBuffer buffer = gran::RHI::Buffer::CreateGpuBuffer(allocator, createInfo);

    gran::RHI::Buffer::MapGpuBuffer(allocator, buffer);
    // memcpy(destination_data, creation.initial_data, static_cast<size_t>(image_size));
    // gran::RHI::Buffer::WriteToGpuBuffer(allocator, buffer);
    gran::RHI::Buffer::UnmapGpuBuffer(allocator, buffer);

    gran::RHI::Buffer::DestroyGpuBuffer(allocator, buffer);
}

static void StorageDeviceBufferTesting(const gran::GpuAllocator& allocator)
{
    const VkDeviceSize bufferSize = 65536;
    std::vector<std::uint8_t> bufferData(bufferSize);
    for (auto& bufferByte : bufferData)
    {
        bufferByte = static_cast<std::uint8_t>(rand() % 256);
    }

    gran::GpuBufferCreateInfo createInfo {};
    createInfo.m_AllocSize = 65536;
    createInfo.m_BufferUsageFlags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
    // createInfo.m_MemoryUsage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    // createInfo.m_AllocFlags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;

    gran::GpuBuffer buffer = gran::RHI::Buffer::CreateGpuBuffer(allocator, createInfo);

    gran::RHI::Buffer::MapGpuBuffer(allocator, buffer);
    // memcpy(destination_data, creation.initial_data, static_cast<size_t>(image_size));
    // gran::RHI::Buffer::UnmapGpuBuffer(allocator, buffer);

    gran::RHI::Buffer::DestroyGpuBuffer(allocator, buffer);
}
*/