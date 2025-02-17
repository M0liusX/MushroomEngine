#include "MeSimple2DRenderSystem.hpp"
#include "MePolygon.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <memory>
#include <array>

namespace me {

struct Simple2DPushConstantData {
   glm::vec3 color{};
};

static std::shared_ptr<MePolygon> poly;
static Simple2DPushConstantData push = { glm::vec3{1.0f, 1.0f, 0.0f} };
static const std::vector<MePolygon::Vertex2D> vertices = { MePolygon::Vertex2D{glm::vec2{-1.0f, -1.0f}},
                                                           MePolygon::Vertex2D{glm::vec2{-1.0f, 1.0f}},
                                                           MePolygon::Vertex2D{glm::vec2{1.0f, -1.0f}} };

MeSimple2DRenderSystem::MeSimple2DRenderSystem(MeDevice& device,
                                          VkRenderPass renderPass,
                                          VkDescriptorSetLayout desciptorSetLayout) :
   meDevice(device)
{
   poly = std::make_shared<MePolygon>(device, vertices);
   createPipelineLayout(desciptorSetLayout);
   createPipeline(renderPass);
}


MeSimple2DRenderSystem::~MeSimple2DRenderSystem()
{
   poly.reset(); // reset for now
   vkDestroyPipelineLayout(meDevice.device(), pipelineLayout, nullptr);
}


void MeSimple2DRenderSystem::createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout)
{   
   VkPushConstantRange pushConstantRange{};
   pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
   pushConstantRange.offset = 0;
   pushConstantRange.size = sizeof(Simple2DPushConstantData);

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


void MeSimple2DRenderSystem::createPipeline(VkRenderPass renderPass)
{
   assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

   std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
   std::vector<VkVertexInputBindingDescription> bindingDescriptions{1};

   /*                               loc bind format                   offset */
   attributeDescriptions.push_back({ 0,  0,  VK_FORMAT_R32G32_SFLOAT,   0 });
   bindingDescriptions[0].binding = 0;
   bindingDescriptions[0].stride = 8;
   bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

   PipelineConfigInfo pipelineConfig{};
   MePipeline::defaultPipelineConfigInfo(pipelineConfig);
   pipelineConfig.renderPass = renderPass;
   pipelineConfig.pipelineLayout = pipelineLayout;
   pipelineConfig.attributeDescriptions = attributeDescriptions;
   pipelineConfig.bindingDescriptions = bindingDescriptions;

   mePipeline = std::make_unique<MePipeline>(meDevice,
      "shaders/bin/simple2d.vert.spv",
      "shaders/bin/simple2d.frag.spv",
      pipelineConfig);
}


void MeSimple2DRenderSystem::renderGameObjects(FrameInfo& frameInfo)
{
   mePipeline->bind(frameInfo.commandBuffer);

   vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipelineLayout,
                           0, 1,
                           &frameInfo.desciptorSet, 0, nullptr);

   //for (auto& kv : frameInfo.gameObjects) {
   vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      0, sizeof(Simple2DPushConstantData), &push);
   poly->bind(frameInfo.commandBuffer);
   poly->draw(frameInfo.commandBuffer);
   //}
}


}  // namespace me
