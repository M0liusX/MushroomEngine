#pragma once

#include "MeRenderSystemsUtility.hpp"

#include <memory>

namespace me {

   class MeSimpleRenderSystem {
   public:
      MeSimpleRenderSystem(MeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout desciptorSetLayout);
      ~MeSimpleRenderSystem();

      MeSimpleRenderSystem(const MeSimpleRenderSystem&) = delete;
      MeSimpleRenderSystem& operator=(const MeSimpleRenderSystem&) = delete;
      void renderGameObjects(FrameInfo& frameInfo);
   private:
      void createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout);
      void createPipeline(VkRenderPass renderPass);

      MeDevice& meDevice;

      std::unique_ptr<MePipeline> mePipeline;
      VkPipelineLayout pipelineLayout;
      // std::unique_ptr<MeDescriptorSetLayout> renderSystemLayout;
   };

}  // namespace me
