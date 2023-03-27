#pragma once

#include "MeDevice.hpp"
#include "MeWindow.hpp"
#include "MeSwapChain.hpp"

#include <memory>
#include <vector>
#include <cassert>


namespace me {

   class MeRenderer {
   public:
      MeRenderer(MeWindow &window, MeDevice &device);
      ~MeRenderer();

      MeRenderer(const MeRenderer&) = delete;
      MeRenderer& operator=(const MeRenderer&) = delete;

      VkRenderPass getSwapChainRenderPass() const { return meSwapChain->getRenderPass(); }
      float getAspectRatio() const { return meSwapChain->extentAspectRatio(); }
      bool isFrameInProgress() const { return isFrameStarted; }

      VkCommandBuffer getCurrentCommandBuffer() const {
         assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
         return commandBuffers[currentFrameIndex];
      }

      int getFrameIndex() const {
         assert(isFrameStarted && "Cannot get frame index when frame not in progress");
         return currentFrameIndex;
      }

      VkCommandBuffer beginFrame();
      void endFrame();
      void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
      void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

   private:
      void createCommandBuffers();
      void freeCommandBuffers();
      void recreateSwapChain();

      MeWindow &meWindow;
      MeDevice &meDevice;;
      std::unique_ptr<MeSwapChain> meSwapChain;
      std::vector<VkCommandBuffer> commandBuffers;

      uint32_t currentImageIndex{ 0 };
      int currentFrameIndex{ 0 };
      bool isFrameStarted{ false };
   };

}  // namespace me