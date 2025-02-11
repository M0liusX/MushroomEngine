#pragma once

#include "MeRenderSystemsUtility.hpp"

#include <memory>

namespace me {

   class MeSimple2DRenderSystem {
   public:
       MeSimple2DRenderSystem(MeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout desciptorSetLayout);
      ~MeSimple2DRenderSystem();

      MeSimple2DRenderSystem(const MeSimple2DRenderSystem&) = delete;
      MeSimple2DRenderSystem& operator=(const MeSimple2DRenderSystem&) = delete;
      void renderGameObjects(FrameInfo& frameInfo);
   private:
      void createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout);
      void createPipeline(VkRenderPass renderPass);

      MeDevice& meDevice;

      std::unique_ptr<MePipeline> mePipeline;
      VkPipelineLayout pipelineLayout;
   };

}  // namespace me
