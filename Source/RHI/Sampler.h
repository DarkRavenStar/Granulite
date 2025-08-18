#include "vulkan/vulkan.h"

// Forward declaration
namespace gran
{
	struct Device;
} // namespace gran

namespace gran
{
	struct GpuSamplerCreateInfo
	{
		VkSamplerCreateInfo m_CreateInfo {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.minLod = 0,
			.maxLod = 16.f,
		};

		VkSamplerReductionMode m_ReductionMode = VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE;
	};

	struct GpuSampler
	{
		VkSampler m_Sampler = nullptr;
	};
} // namespace gran

namespace gran::RHI::Sampler
{
	gran::GpuSampler CreateGpuSampler(const gran::Device& device, const gran::GpuSamplerCreateInfo createInfo);

	void DestroyGpuSampler(const gran::Device& device, const gran::GpuSampler sampler);

} // namespace gran::RHI::Sampler