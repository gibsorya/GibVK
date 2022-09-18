#include <vk_initializers.h>
namespace vkinit
{
    VkCommandPoolCreateInfo commandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
    {
        VkCommandPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = flags;
        info.queueFamilyIndex = queueFamilyIndex;

        return info;
    }
    VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
    {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.pNext = nullptr;

        info.commandPool = pool;
        info.commandBufferCount = count;
        info.level = level;
        return info;
    }

    VkFramebufferCreateInfo framebufferCreateInfo(VkRenderPass renderPass, VkExtent2D extent)
    {
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.pNext = nullptr;

        info.renderPass = renderPass;
        info.width = extent.width;
        info.height = extent.height;
        info.layers = 1;
        info.attachmentCount = 1;

        return info;
    }

    VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags /*= 0*/)
    {
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.pNext = nullptr;

        info.pInheritanceInfo = nullptr;
        info.flags = flags;
        return info;
    }

    VkRenderPassBeginInfo renderPassBeginInfo(VkRenderPass renderPass, VkExtent2D extent, VkFramebuffer framebuffer)
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.pNext = nullptr;
        info.renderPass = renderPass;
        info.framebuffer = framebuffer;
        info.renderArea.offset.x = 0;
        info.renderArea.offset.y = 0;
        info.renderArea.extent = extent;

        return info;
    }

    VkSubmitInfo submitInfo(VkCommandBuffer* cmd)
    {
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.pNext = nullptr;

        info.waitSemaphoreCount = 0;
        info.pWaitSemaphores = nullptr;
        info.pWaitDstStageMask = nullptr;
        info.commandBufferCount = 1;
        info.pCommandBuffers = cmd;
        info.signalSemaphoreCount = 0;
        info.pSignalSemaphores = nullptr;

        return info;
    }

    VkPresentInfoKHR presentInfo()
    {
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.pNext = nullptr;

        info.swapchainCount = 0;
        info.pSwapchains = nullptr;
        info.pWaitSemaphores = nullptr;
        info.waitSemaphoreCount = 0;
        info.pImageIndices = nullptr;

        return info;
    }

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
    {
        VkPipelineShaderStageCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.pName = nullptr;
        info.stage = stage;
        info.pName = "main";
        info.module = shaderModule;

        return info;
    }

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo() {
            VkPipelineVertexInputStateCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            info.pNext = nullptr;

            //no vertex bindings or attributes
            info.vertexBindingDescriptionCount = 0;
            info.vertexAttributeDescriptionCount = 0;
            return info;
    }

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(VkPrimitiveTopology topology) {
        VkPipelineInputAssemblyStateCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        info.pNext = nullptr;

        info.topology = topology;
        //we are not going to use primitive restart on the entire tutorial so leave it on false
        info.primitiveRestartEnable = VK_FALSE;
        return info;
    }

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo(VkPolygonMode mode)
    {
        VkPipelineRasterizationStateCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        info.pNext = nullptr;
        info.polygonMode = mode;
        info.cullMode = VK_CULL_MODE_NONE;
        info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        info.depthBiasEnable = VK_FALSE;
        info.rasterizerDiscardEnable = VK_FALSE;
        info.depthClampEnable = VK_FALSE;
        info.depthBiasConstantFactor = 0.0f;
        info.depthBiasSlopeFactor = 0.0f;
        info.depthBiasClamp = 0.0f;
        info.lineWidth = 1.0f;

        return info;
    }

    VkPipelineMultisampleStateCreateInfo multisampleCreateInfo()
    {
        VkPipelineMultisampleStateCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        info.pNext = nullptr;

        info.sampleShadingEnable = VK_FALSE;
        info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        info.minSampleShading = 1.0f;
        info.pSampleMask = nullptr;
        info.alphaToCoverageEnable = VK_FALSE;
        info.alphaToOneEnable = VK_FALSE;

        return info;
    }

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

        return colorBlendAttachment;
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo()
    {
        VkPipelineLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
		info.setLayoutCount = 0;
		info.pSetLayouts = nullptr;
		info.pushConstantRangeCount = 0;
		info.pPushConstantRanges = nullptr;

        return info;
    }

    VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags)
    {
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = flags;
        return fenceCreateInfo;
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo(VkSemaphoreCreateFlags flags)
    {
        VkSemaphoreCreateInfo semCreateInfo = {};
        semCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semCreateInfo.pNext = nullptr;
        semCreateInfo.flags = flags;
        return semCreateInfo;
    }
}