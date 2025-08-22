#include "RHI/RenderPass.h"

#include "RHI/CommonHelper.h"
#include "RHI/Device.h"
#include "RHI/Memory.h"
#include "volk.h"

gran::RenderAttachmentInfo gran::RHI::RenderPass::CreateColorAttachmentInfo(const gran::RenderAttachmentCreateInfo& createInfo)
{
	RenderAttachmentInfo info;

	VkRenderingAttachmentInfo& colorAttachment = info.m_AttachmentInfo;
	colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colorAttachment.pNext = nullptr;

	colorAttachment.imageView = createInfo.m_ImageView;
	colorAttachment.imageLayout = createInfo.m_ImageLayout;
	colorAttachment.loadOp = createInfo.m_DoClearValue ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.clearValue = createInfo.m_ClearValue;

	return info;
};

gran::RenderAttachmentInfo gran::RHI::RenderPass::CreateDepthAttachmentInfo(const gran::RenderAttachmentCreateInfo& createInfo)
{
	RenderAttachmentInfo info;

	VkRenderingAttachmentInfo& depthAttachment = info.m_AttachmentInfo;
	depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	depthAttachment.pNext = nullptr;

	depthAttachment.imageView = createInfo.m_ImageView;
	depthAttachment.imageLayout = createInfo.m_ImageLayout;
	depthAttachment.loadOp = createInfo.m_DoClearValue ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.clearValue.depthStencil.depth = 0.f;

	return info;
};

gran::RenderingInfo gran::RHI::RenderPass::CreateRenderingInfo(const RenderingCreateInfo& createInfo)
{
	gran::RenderingInfo info;
	VkRenderingInfo& renderInfo = info.m_RenderInfo;
	renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderInfo.pNext = nullptr;

	// renderInfo.renderArea.extent.width = swapchain.m_SwapchainExtent.width;
	// renderInfo.renderArea.extent.height = swapchain.m_SwapchainExtent.height;
	renderInfo.renderArea = VkRect2D { VkOffset2D { 0, 0 }, createInfo.m_RenderExtent };
	renderInfo.layerCount = 1;
	renderInfo.colorAttachmentCount = 1;
	renderInfo.pColorAttachments = createInfo.m_ColorAttachment;
	renderInfo.pDepthAttachment = createInfo.m_DepthAttachment;
	renderInfo.pStencilAttachment = createInfo.m_StencilAttachment;

	return info;
};