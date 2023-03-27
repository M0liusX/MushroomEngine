#include "MeBillboardRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <memory>
#include <array>

namespace me {


//struct SimplePushConstantData {
//   glm::mat4 modelMatrix{ 1.f };
//   glm::mat4 normalMatrix{ 1.f };
//   // alignas(16) glm::vec3 color;
//};


MeBillboardRenderSystem::MeBillboardRenderSystem(MeDevice& device,
                                                 VkRenderPass renderPass,
                                                 VkDescriptorSetLayout desciptorSetLayout) :
   meDevice(device)
{
   createPipelineLayout(desciptorSetLayout);
   createPipeline(renderPass);
}


MeBillboardRenderSystem::~MeBillboardRenderSystem()
{
   vkDestroyPipelineLayout(meDevice.device(), pipelineLayout, nullptr);
}


void MeBillboardRenderSystem::createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout)
{   
   //VkPushConstantRange pushConstantRange{};
   //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
   //pushConstantRange.offset = 0;
   //pushConstantRange.size = sizeof(SimplePushConstantData);

   std::vector<VkDescriptorSetLayout> desciptorSetLayouts{ desciptorSetLayout };

   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(desciptorSetLayouts.size());
   pipelineLayoutInfo.pSetLayouts = desciptorSetLayouts.data();
   pipelineLayoutInfo.pushConstantRangeCount = 0;
   pipelineLayoutInfo.pPushConstantRanges = nullptr; //&pushConstantRange;
   if (vkCreatePipelineLayout(meDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipleine layout!");
   }
}


void MeBillboardRenderSystem::createPipeline(VkRenderPass renderPass)
{
   assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

   PipelineConfigInfo pipelineConfig{};
   MePipeline::defaultPipelineConfigInfo(pipelineConfig);
   pipelineConfig.renderPass = renderPass;
   pipelineConfig.pipelineLayout = pipelineLayout;
   mePipeline = std::make_unique<MePipeline>(meDevice,
      "shaders/bin/billboard_shader.vert.spv",
      "shaders/bin/billboard_shader.frag.spv",
      pipelineConfig);
}


void MeBillboardRenderSystem::renderGameObjects(FrameInfo& frameInfo)
{
   mePipeline->bind(frameInfo.commandBuffer);

   vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipelineLayout,
                           0, 1,
                           &frameInfo.desciptorSet, 0, nullptr);
   vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}


}  // namespace me
