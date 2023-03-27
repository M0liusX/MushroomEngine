#include "MeGreasePaintRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <memory>
#include <array>

namespace me {


struct SimplePushConstantData {
   glm::mat4 modelMatrix{ 1.f };
   // alignas(16) glm::vec3 color;
};


MeGreasePaintRenderSystem::MeGreasePaintRenderSystem(MeDevice& device,
                                                     VkRenderPass renderPass,
                                                     VkDescriptorSetLayout desciptorSetLayout) :
   meDevice(device)
{
   createPipelineLayout(desciptorSetLayout);
   createPipeline(renderPass);
}


MeGreasePaintRenderSystem::~MeGreasePaintRenderSystem()
{
   vkDestroyPipelineLayout(meDevice.device(), pipelineLayout, nullptr);
}


void MeGreasePaintRenderSystem::createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout)
{   
   VkPushConstantRange pushConstantRange{};
   pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
   pushConstantRange.offset = 0;
   pushConstantRange.size = sizeof(SimplePushConstantData);

   std::vector<VkDescriptorSetLayout> desciptorSetLayouts{ desciptorSetLayout };

   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(desciptorSetLayouts.size());
   pipelineLayoutInfo.pSetLayouts = desciptorSetLayouts.data();
   pipelineLayoutInfo.pushConstantRangeCount = 1;
   pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
   if (vkCreatePipelineLayout(meDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipleine layout!");
   }
}


void MeGreasePaintRenderSystem::createPipeline(VkRenderPass renderPass)
{
   assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

   PipelineConfigInfo pipelineConfig{};
   MePipeline::defaultPipelineConfigInfo(pipelineConfig);
   pipelineConfig.renderPass = renderPass;
   pipelineConfig.pipelineLayout = pipelineLayout;
   pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
   //pipelineConfig.attributeDescriptions = MePhysicsModel::Vertex::getAttributeDescription();
   //pipelineConfig.bindingDescriptions = MePhysicsModel::Vertex::getBindingDescription();
   mePipeline = std::make_unique<MePipeline>(meDevice,
      "shaders/bin/physics.vert.spv",
      "shaders/bin/physics.frag.spv",
      pipelineConfig);
}


void MeGreasePaintRenderSystem::renderGreasePaint(FrameInfo& frameInfo)
{
   mePipeline->bind(frameInfo.commandBuffer);

   vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipelineLayout,
                           0, 1,
                           &frameInfo.desciptorSet, 0, nullptr);

   auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();
   for (auto& kv : frameInfo.gameObjects) {
      auto& obj = kv.second;
      if (obj.pModel == nullptr) continue;
      if (obj.physicsObject == -1) continue;

      SimplePushConstantData push{};
      push.modelMatrix = frameInfo.physicsServer.getObject(obj.physicsObject).getCollider()->GetMat4(obj.transform);

      vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
         VK_SHADER_STAGE_VERTEX_BIT,
         0, sizeof(SimplePushConstantData), &push);
      obj.pModel->bind(frameInfo.commandBuffer);
      obj.pModel->draw(frameInfo.commandBuffer);
   }
}


}  // namespace me
