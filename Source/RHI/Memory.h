#pragma once

#include "vulkan/vulkan.h"

namespace gran
{
	struct Device;
} // namespace gran

typedef struct VmaAllocator_T* VmaAllocator;

namespace gran
{
	struct GpuAllocator
	{
		VmaAllocator m_Allocator = VK_NULL_HANDLE;
	};

} // namespace gran

namespace gran::RHI::Memory
{
	/*
	-# If you just want to find memory type index that meets your requirements, you
	   can use function: vmaFindMemoryTypeIndexForBufferInfo(),
	   vmaFindMemoryTypeIndexForImageInfo(), vmaFindMemoryTypeIndex().

	-# If you want to allocate a region of device memory without association with any
	    specific image or buffer, you can use function vmaAllocateMemory(). Usage of
	    this function is not recommended and usually not needed.

	    vmaAllocateMemoryPages() function is also provided for creating multiple allocations at once,
	    which may be useful for sparse binding.

	*/

	// TODO: Need to update the VmaAllocation and VmaAllocationInfo info if doing defragmentation
	// allocator.IsMemoryTypeNonCoherent();
	// vmaFindMemoryTypeIndexForBufferInfo()
	// vmaFindMemoryTypeIndexForImageInfo()

	void SetupAllocator(const gran::Device& device, gran::GpuAllocator& outAllocator);

	void CleanupAllocator(const gran::Device& device, gran::GpuAllocator& allocator);

	VkMemoryPropertyFlags GetMemoryTypeProperties(const gran::GpuAllocator& allocator, uint32_t memoryType);

	void CheckAndAssertNonMappingMemory(const gran::GpuAllocator& allocator, uint32_t memoryType);

	// If we want to find and allocate memory manually
	uint32_t SelectMemoryType(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags);

} // namespace gran::RHI::Memory
