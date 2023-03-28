#pragma once

#include "MeDevice.hpp"

// std
#include <memory>

namespace me {
   class MeTexture {
   public:
      MeTexture(MeDevice& device, const std::string& textureFilepath);
      MeTexture(
         MeDevice& device,
         VkFormat format,
         VkExtent3D extent,
         VkImageUsageFlags usage,
         VkSampleCountFlagBits sampleCount);
      ~MeTexture();

      // delete copy constructors
      MeTexture(const MeTexture&) = delete;
      MeTexture& operator=(const MeTexture&) = delete;

      VkImageView imageView() const { return mTextureImageView; }
      VkSampler sampler() const { return mTextureSampler; }
      VkImage getImage() const { return mTextureImage; }
      VkImageView getImageView() const { return mTextureImageView; }
      VkDescriptorImageInfo getImageInfo() const { return mDescriptor; }
      VkImageLayout getImageLayout() const { return mTextureLayout; }
      VkExtent3D getExtent() const { return mExtent; }
      VkFormat getFormat() const { return mFormat; }

      void updateDescriptor();
      void transitionLayout(
         VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);

      static std::unique_ptr<MeTexture> createTextureFromFile(
         MeDevice& device, const std::string& filepath);

   private:
      void createTextureImage(const std::string& filepath);
      void createTextureImageView(VkImageViewType viewType);
      void createTextureSampler();

      VkDescriptorImageInfo mDescriptor{};

      MeDevice& mDevice;
      VkImage mTextureImage = nullptr;
      VkDeviceMemory mTextureImageMemory = nullptr;
      VkImageView mTextureImageView = nullptr;
      VkSampler mTextureSampler = nullptr;
      VkFormat mFormat;
      VkImageLayout mTextureLayout;
      uint32_t mMipLevels{ 1 };
      uint32_t mLayerCount{ 1 };
      VkExtent3D mExtent{};
   };

}  // namespace me
