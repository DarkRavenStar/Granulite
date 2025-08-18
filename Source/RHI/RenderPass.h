#pragma once

#include "vulkan/vulkan.h"

namespace gran
{
	struct RenderAttachmentCreateInfo
	{
		VkImageView m_ImageView;
		VkImageLayout m_ImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		bool m_DoClearValue = true;
		VkClearValue m_ClearValue { 0 };
	};

	struct RenderAttachmentInfo
	{
		VkRenderingAttachmentInfo m_AttachmentInfo;
	};
} // namespace gran

namespace gran::RHI::RenderPass
{
	gran::RenderAttachmentInfo CreateColorAttachmentInfo(const gran::RenderAttachmentCreateInfo& createInfo);

	gran::RenderAttachmentInfo CreateDepthAttachmentInfo(const gran::RenderAttachmentCreateInfo& createInfo);

} // namespace gran::RHI::RenderPass
