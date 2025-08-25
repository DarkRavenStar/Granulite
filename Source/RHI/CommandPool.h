#include "vulkan/vulkan.h"

// Forward declaration
namespace gran
{
	struct Device;
	struct GpuAllocator;
} // namespace gran

namespace gran
{
	struct GpuCommandPoolCreateInfo
	{
		VkCommandPoolCreateInfo m_CreateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			//.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
			//.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			//.queueFamilyIndex
		};
	};

	struct GpuCommandPool
	{
		VkCommandPool m_CommandPool;
	};

} // namespace gran

namespace gran::RHI::CommandPool
{
	GpuCommandPool CreateGpuCommandPool(
	    const gran::Device& device,
	    const gran::GpuCommandPoolCreateInfo& createInfo);

	void ResetGpuCommandPool(const gran::Device& device, const GpuCommandPool& pool);

	void DestroyGpuCommandPool(const gran::Device& device, const GpuCommandPool& pool);

} // namespace gran::RHI::CommandPool
