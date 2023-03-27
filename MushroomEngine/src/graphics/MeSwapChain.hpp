#pragma warning(disable : 26812) // Suppress: The enum type type-name is unscoped. Prefer enum class over enum
#pragma once

#include "MeDevice.hpp"

#include <vulkan/vulkan.h>

#include <memory>
#include <string>
#include <vector>

namespace me {

class MeSwapChain {
public:
   static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

   MeSwapChain(MeDevice &deviceRef, VkExtent2D windowExtent);
   MeSwapChain(MeDevice& deviceRef, VkExtent2D windowExtent, std::shared_ptr<MeSwapChain> prev);
   ~MeSwapChain();

   MeSwapChain(const MeSwapChain&) = delete;
   MeSwapChain& operator=(const MeSwapChain &) = delete;

   VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
   VkRenderPass getRenderPass() { return renderPass; }
   VkImageView getImageView(int index) { return swapChainImageViews[index]; }
   size_t imageCount() { return swapChainImages.size(); }
   VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
   VkExtent2D getSwapChainExtent() { return swapChainExtent; }
   uint32_t width() { return swapChainExtent.width; }
   uint32_t height() { return swapChainExtent.height; }

   float extentAspectRatio() {
      return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
   }
   VkFormat findDepthFormat();

   VkResult acquireNextImage(uint32_t *imageIndex);
   VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

   bool compareSwapFormats(const MeSwapChain& swapChain) const {
      return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
             swapChain.swapChainImageFormat == swapChainImageFormat;
   }

private:
   void init();
   void createSwapChain();
   void createImageViews();
   void createDepthResources();
   void createRenderPass();
   void createFramebuffers();
   void createSyncObjects();

  // Helper functions
   VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
   VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
   VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

   VkFormat swapChainImageFormat;
   VkFormat swapChainDepthFormat;
   VkExtent2D swapChainExtent;

   std::vector<VkFramebuffer> swapChainFramebuffers;
   VkRenderPass renderPass;

   std::vector<VkImage> depthImages;
   std::vector<VkDeviceMemory> depthImageMemorys;
   std::vector<VkImageView> depthImageViews;
   std::vector<VkImage> swapChainImages;
   std::vector<VkImageView> swapChainImageViews;

   MeDevice &device;
   VkExtent2D windowExtent;

   VkSwapchainKHR swapChain;
   std::shared_ptr<MeSwapChain> oldSwapChain;

   std::vector<VkSemaphore> imageAvailableSemaphores;
   std::vector<VkSemaphore> renderFinishedSemaphores;
   std::vector<VkFence> inFlightFences;
   std::vector<VkFence> imagesInFlight;
   size_t currentFrame = 0;
};

}  // namespace me
