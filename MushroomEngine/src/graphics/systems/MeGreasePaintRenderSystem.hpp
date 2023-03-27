#pragma once

#include "MeRenderSystemsUtility.hpp"

#include <memory>

namespace me {

   class MeGreasePaintRenderSystem {
   public:
      MeGreasePaintRenderSystem(MeDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout desciptorSetLayout);
      ~MeGreasePaintRenderSystem();

      MeGreasePaintRenderSystem(const MeGreasePaintRenderSystem&) = delete;
      MeGreasePaintRenderSystem& operator=(const MeGreasePaintRenderSystem&) = delete;
      void renderGreasePaint(FrameInfo& frameInfo);
   private:
      void createPipelineLayout(VkDescriptorSetLayout desciptorSetLayout);
      void createPipeline(VkRenderPass renderPass);

      MeDevice& meDevice;

      std::unique_ptr<MePipeline> mePipeline;
      VkPipelineLayout pipelineLayout;

   };

}  // namespace me
