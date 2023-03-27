#pragma once

#include "MeRenderSystemsUtility.hpp"

#include <memory>

namespace me {

   class MePhysicsRenderSystem {
   public:
      MePhysicsRenderSystem(MeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout desciptorSetLayout);
      ~MePhysicsRenderSystem();

      MePhysicsRenderSystem(const MePhysicsRenderSystem&) = delete;
      MePhysicsRenderSystem& operator=(const MePhysicsRenderSystem&) = delete;
      void renderPhysicsObjects(FrameInfo& frameInfo);
   private:
      void createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout);
      void createPipeline(VkRenderPass renderPass);

      MeDevice& meDevice;

      std::unique_ptr<MePipeline> mePipeline;
      VkPipelineLayout pipelineLayout;

   };

}  // namespace me
