// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>

namespace vkinit
{
	VkCommandPoolCreateInfo commandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
	VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	VkRenderPassCreateInfo renderPassCreateInfo();
	VkFramebufferCreateInfo framebufferCreateInfo(VkRenderPass renderPass, VkExtent2D extent);
	VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
	VkRenderPassBeginInfo renderPassBeginInfo(VkRenderPass renderPass, VkExtent2D extent, VkFramebuffer framebuffer);
	VkSubmitInfo submitInfo(VkCommandBuffer* cmd);
	VkPresentInfoKHR presentInfo();
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(VkPrimitiveTopology topology);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode mode);
	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo();
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState();
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo();
	VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0);
	VkSemaphoreCreateInfo semaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
	VkImageCreateInfo imageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
	VkImageViewCreateInfo imageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
	VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo(bool bDepthTest, bool bDepthWrite, VkCompareOp compareOp);
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
	VkWriteDescriptorSet writeDescriptorBuffer(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo , uint32_t binding);
}
