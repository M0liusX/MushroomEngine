#pragma once

#include "MeRenderSystemsUtility.hpp"

#include <memory>

namespace me {

   class MeBillboardRenderSystem {
   public:
      MeBillboardRenderSystem(MeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout desciptorSetLayout);
      ~MeBillboardRenderSystem();

      MeBillboardRenderSystem(const MeBillboardRenderSystem&) = delete;
      MeBillboardRenderSystem& operator=(const MeBillboardRenderSystem&) = delete;
      void renderGameObjects(FrameInfo& frameInfo);
   private:
      void createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout);
      void createPipeline(VkRenderPass renderPass);

      MeDevice& meDevice;

      std::unique_ptr<MePipeline> mePipeline;
      VkPipelineLayout pipelineLayout;

   };

}  // namespace me
