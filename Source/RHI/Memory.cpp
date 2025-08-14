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
	// allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	//  VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT that will let us use GPU pointers later when we need them.
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	// allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT |
	//                             VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT |
	//                             VMA_ALLOCATOR_CREATE_KHR_EXTERNAL_MEMORY_WIN32_BIT;

	VmaVulkanFunctions vulkanFunctions;
	VkResult res = vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, &vulkanFunctions);
	allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

	vmaCreateAllocator(&allocatorCreateInfo, &outAllocator.m_Allocator);
}

void gran::RHI::Memory::CleanupAllocator(const gran::Device& device, gran::GpuAllocator& allocator)
{
	vmaDestroyAllocator(allocator.m_Allocator);
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