#include "MeSimpleRenderSystem.hpp"
#include "MeModel.hpp"

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
   glm::mat4 normalMatrix{ 1.f };
   // alignas(16) glm::vec3 color;
};


MeSimpleRenderSystem::MeSimpleRenderSystem(MeDevice& device,
                                          VkRenderPass renderPass,
                                          VkDescriptorSetLayout desciptorSetLayout) :
   meDevice(device)
{
   createPipelineLayout(desciptorSetLayout);
   createPipeline(renderPass);
}


MeSimpleRenderSystem::~MeSimpleRenderSystem()
{
   vkDestroyPipelineLayout(meDevice.device(), pipelineLayout, nullptr);
}


void MeSimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout)
{   
   VkPushConstantRange pushConstantRange{};
   pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
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


void MeSimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
   assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

   PipelineConfigInfo pipelineConfig{};
   MePipeline::defaultPipelineConfigInfo(pipelineConfig);
   pipelineConfig.renderPass = renderPass;
   pipelineConfig.pipelineLayout = pipelineLayout;
   pipelineConfig.attributeDescriptions = MeModel::Vertex::getAttributeDescription();
   pipelineConfig.bindingDescriptions = MeModel::Vertex::getBindingDescription();
   //pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
   mePipeline = std::make_unique<MePipeline>(meDevice,
      "shaders/bin/simple_shader.vert.spv",
      "shaders/bin/simple_shader.frag.spv",
      pipelineConfig);
}


void MeSimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo)
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
      if (obj.model == nullptr) continue;
      //obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.001f, glm::two_pi<float>());
      //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.0005f, glm::two_pi<float>());
      SimplePushConstantData push{};
      push.modelMatrix = obj.transform.mat4();
      push.normalMatrix = obj.transform.normalMatrix();

      vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
         0, sizeof(SimplePushConstantData), &push);
      obj.model->bind(frameInfo.commandBuffer);
      obj.model->draw(frameInfo.commandBuffer);
   }
}


}  // namespace me
