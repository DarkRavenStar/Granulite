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
	void SetupAllocator(const gran::Device& device, gran::GpuAllocator& outAllocator);

	void CleanupAllocator(const gran::Device& device, gran::GpuAllocator& allocator);

	// If we want to find and allocate memory manually
	uint32_t SelectMemoryType(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags);

} // namespace gran::RHI::Memory
