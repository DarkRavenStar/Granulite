#include "RHI/Sampler.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "volk.h"

gran::GpuSampler gran::RHI::Sampler::CreateGpuSampler(const gran::Device& device, const gran::GpuSamplerCreateInfo createInfo)
{
	VkSamplerCreateInfo samplerCreateInfo = createInfo.m_CreateInfo;
	VkSamplerMipmapMode mipmapMode = samplerCreateInfo.mipmapMode;

	samplerCreateInfo.anisotropyEnable = (mipmapMode & VK_SAMPLER_MIPMAP_MODE_LINEAR);
	samplerCreateInfo.maxAnisotropy = (mipmapMode & VK_SAMPLER_MIPMAP_MODE_LINEAR) ? 4.0f : 1.0f;

	VkSamplerReductionModeCreateInfoEXT createInfoReduction;
	createInfoReduction.sType = VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT;

	if (createInfo.m_ReductionMode != VK_SAMPLER_REDUCTION_MODE_WEIGHTED_AVERAGE_EXT)
	{
		createInfoReduction.reductionMode = createInfo.m_ReductionMode;

		samplerCreateInfo.pNext = &createInfoReduction;
	}

	GpuSampler sampler;
	VK_CHECK(vkCreateSampler(device.m_Device, &samplerCreateInfo, nullptr, &sampler.m_Sampler));
	return sampler;
}

void gran::RHI::Sampler::DestroyGpuSampler(const gran::Device& device, const gran::GpuSampler sampler)
{
	vkDestroySampler(device.m_Device, sampler.m_Sampler, nullptr);
}