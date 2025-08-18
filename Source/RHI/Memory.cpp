#include "RHI/Memory.h"

#include "RHI/Device.h"
#include "vk_mem_alloc.h"
#include "volk.h"
#include <assert.h>

void gran::RHI::Memory::SetupAllocator(const gran::Device& device, gran::GpuAllocator& outAllocator)
{
	VmaAllocatorCreateInfo allocatorCreateInfo = {};
	allocatorCreateInfo.physicalDevice = device.m_PhysicalDevice;
	allocatorCreateInfo.device = device.m_Device;
	allocatorCreateInfo.instance = device.m_Instance;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3; // VK_VERSION_1_4

	// VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT that will let us use GPU pointers later when we need them.
	// For other options look at typedef enum VmaAllocatorCreateFlagBits
	// VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT |
	// VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT |
	// VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT | - Not recommended unless necessary
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

	VmaVulkanFunctions vulkanFunctions;
	VkResult res = vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, &vulkanFunctions);
	allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

	vmaCreateAllocator(&allocatorCreateInfo, &outAllocator.m_Allocator);
}

void gran::RHI::Memory::CleanupAllocator(const gran::Device& device, gran::GpuAllocator& allocator)
{
	vmaDestroyAllocator(allocator.m_Allocator);
}

VkMemoryPropertyFlags gran::RHI::Memory::GetMemoryTypeProperties(const gran::GpuAllocator& allocator, uint32_t memoryType)
{
	VkMemoryPropertyFlags result;
	vmaGetMemoryTypeProperties(allocator.m_Allocator, memoryType, &result);
	// vmaGetAllocationMemoryProperties(allocator.m_Allocator, );
	return result;
}

void gran::RHI::Memory::CheckAndAssertNonMappingMemory(const gran::GpuAllocator& allocator, uint32_t memoryType)
{
	VkMemoryPropertyFlags result;
	vmaGetMemoryTypeProperties(allocator.m_Allocator, memoryType, &result);

	if (!(result & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
	{
		assert(!"Operation not allowed for non-mapping type");
		return;
	}
}

uint32_t gran::RHI::Memory::SelectMemoryType(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags)
{
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((memoryTypeBits & (1 << i)) != 0 && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
		{
			return i;
		}
	}

	assert(!"No compatible memory type found");
	return ~0u;
}