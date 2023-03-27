#include "MeRenderer.hpp"

#include <stdexcept>
#include <memory>
#include <array>

namespace me {


MeRenderer::MeRenderer(MeWindow &window,
                       MeDevice &device) :
   meWindow(window), meDevice(device)
{
   recreateSwapChain();
   createCommandBuffers();
}


MeRenderer::~MeRenderer()
{
   freeCommandBuffers();
}


void MeRenderer::recreateSwapChain()
{
   auto extent = meWindow.getExtent();
   while (extent.width == 0 || extent.height == 0) {
      extent = meWindow.getExtent();
      glfwWaitEvents();
   }
   vkDeviceWaitIdle(meDevice.device());
   if (meSwapChain == nullptr) {
      meSwapChain = std::make_unique<MeSwapChain>(meDevice, extent);
   }
   else {
      //meSwapChain = nullptr; // May not be able to have 2 swapchains at the same time!
      std::shared_ptr<MeSwapChain> oldSwapChain = std::move(meSwapChain);
      meSwapChain = std::make_unique<MeSwapChain>(meDevice, extent, oldSwapChain);

      if (!oldSwapChain->compareSwapFormats(*meSwapChain.get())) {
         throw std::runtime_error("Swap chain image(or depth) format has changed!");
      }
   }
}


void MeRenderer::createCommandBuffers()
{
   commandBuffers.resize(MeSwapChain::MAX_FRAMES_IN_FLIGHT);

   VkCommandBufferAllocateInfo allocInfo{};
   allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
   allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
   allocInfo.commandPool = meDevice.getCommandPool();
   allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
   if (vkAllocateCommandBuffers(meDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
   }
}


void MeRenderer::freeCommandBuffers()
{
   vkFreeCommandBuffers(meDevice.device(), meDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
   commandBuffers.clear();
}


VkCommandBuffer MeRenderer::beginFrame()
{
   assert(!isFrameStarted && "Can't call beginFrame while already in progress");

   auto result = meSwapChain->acquireNextImage(&currentImageIndex);

   if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return nullptr;
   }

   if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to aquire swap chain image!");
   }

   isFrameStarted = true;
   auto commandBuffer = getCurrentCommandBuffer();
   VkCommandBufferBeginInfo beginInfo{};
   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

   if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
   }
   return commandBuffer;
}


void MeRenderer::endFrame()
{
   assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
   auto commandBuffer = getCurrentCommandBuffer();

   if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
   }

   auto result = meSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
   if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || meWindow.wasWindowResized()) {
      meWindow.resetWindowResizedFlag();
      recreateSwapChain();
   } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
   }

   isFrameStarted = false;
   currentFrameIndex = (currentFrameIndex + 1) % MeSwapChain::MAX_FRAMES_IN_FLIGHT;
}


void MeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
   assert(isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress");
   assert(commandBuffer == getCurrentCommandBuffer() &&
          "Can't begin render pass on command buffer from a different frame");

   VkRenderPassBeginInfo renderPassInfo{};
   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
   renderPassInfo.renderPass = meSwapChain->getRenderPass();
   renderPassInfo.framebuffer = meSwapChain->getFrameBuffer(currentImageIndex);

   renderPassInfo.renderArea.offset = { 0, 0 };
   renderPassInfo.renderArea.extent = meSwapChain->getSwapChainExtent();

   std::array<VkClearValue, 2> clearValues{};
   clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
   clearValues[1].depthStencil = { 1.0f, 0 };
   renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
   renderPassInfo.pClearValues = clearValues.data();

   vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

   VkViewport viewport;
   viewport.x = 0.0f;
   viewport.y = 0.0f;
   viewport.width = static_cast<float>(meSwapChain->getSwapChainExtent().width);
   viewport.height = static_cast<float>(meSwapChain->getSwapChainExtent().height);
   viewport.minDepth = 0.0f;
   viewport.maxDepth = 1.0f;

   VkRect2D scissor;
   scissor.offset = { 0, 0 };
   scissor.extent = { meSwapChain->getSwapChainExtent().width,
                      meSwapChain->getSwapChainExtent().height };

   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}


void MeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
   assert(isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress");
   assert(commandBuffer == getCurrentCommandBuffer() &&
          "Can't end render pass on command buffer from a different frame");
   vkCmdEndRenderPass(commandBuffer);
}

}  // namespace me