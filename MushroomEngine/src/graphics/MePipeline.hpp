#pragma once

#include "MeDevice.hpp"

#include <string>
#include <vector>

namespace me {

struct PipelineConfigInfo{
   PipelineConfigInfo(const PipelineConfigInfo&) = delete;
   PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

   std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
   std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
   VkPipelineViewportStateCreateInfo viewportInfo;
   VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
   VkPipelineRasterizationStateCreateInfo rasterizationInfo;
   VkPipelineMultisampleStateCreateInfo multisampleInfo;
   VkPipelineColorBlendAttachmentState colorBlendAttachment;
   VkPipelineColorBlendStateCreateInfo colorBlendInfo;
   VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
   std::vector<VkDynamicState> dynamicStateEnables;
   VkPipelineDynamicStateCreateInfo dynamicStateInfo;
   VkPipelineLayout pipelineLayout = nullptr;
   VkRenderPass renderPass = nullptr;
   uint32_t subpass = 0;
};

class MePipeline {
public:
   MePipeline(MeDevice& device,
              const std::string& vertFilepath,
              const std::string& frapFilepath,
              const PipelineConfigInfo& configInfo);
   ~MePipeline();
   MePipeline& operator=(const MePipeline&) = delete;

   void bind(VkCommandBuffer commandBuffer);
   static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
   
private:
   static std::vector<char> readFile(const std::string& filepath);
   void createGraphicsPipeline(const std::string& vertFilepath,
                               const std::string& frapFilepath,
                               const PipelineConfigInfo& configInfo);
   void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
   MeDevice& meDevice;
   VkPipeline graphicsPipeline;
   VkShaderModule vertShaderModule;
   VkShaderModule fragShaderModule;
};

}  // namespace me