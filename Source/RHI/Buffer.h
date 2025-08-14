#pragma once
#include "vk_mem_alloc.h"
#include "vulkan/vulkan.h"

// Forward declaration
namespace gran
{
	struct Device;
	struct GpuAllocator;
} // namespace gran

namespace gran
{
	// To help with forward declaration
	// we could use raw pointer if needed
	struct GpuBuffer
	{
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
		VmaAllocationInfo m_AllocInfo;
	};

	struct GpuBufferCreateInfo
	{
		size_t m_AllocSize;
		VkBufferUsageFlags m_BufferUsageFlags;
		VmaMemoryUsage m_MemoryUsage;

		// Memory mapped is good enough for now as default. Got other options based on needs
		// VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		// VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT;
		VmaAllocationCreateFlags m_MemoryAllocTypeFlags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
	};

} // namespace gran

namespace gran::RHI::Buffer
{
	GpuBuffer CreateGpuBuffer(const gran::GpuAllocator& allocator, const gran::GpuBufferCreateInfo& createInfo);

	void DestroyBuffer(const gran::GpuAllocator& allocator, const gran::GpuBuffer& buffer);

	// Get raw GPU pointer to a buffer and pass it for usage inside a shader or other use cases.
	VkDeviceAddress GetBufferAddress(const gran::Device& device, const gran::GpuBuffer& buffer);

} // namespace gran::RHI::Buffer
