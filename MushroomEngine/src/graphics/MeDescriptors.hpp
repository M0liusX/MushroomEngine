#pragma once

#include "MeDevice.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace me {

   class MeDescriptorSetLayout {
   public:
      class Builder {
      public:
         Builder(MeDevice& meDevice) : meDevice{ meDevice } {}

         Builder& addBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1);
         std::unique_ptr<MeDescriptorSetLayout> build() const;

      private:
         MeDevice& meDevice;
         std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
      };

      MeDescriptorSetLayout(
         MeDevice& meDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
      ~MeDescriptorSetLayout();
      MeDescriptorSetLayout(const MeDescriptorSetLayout&) = delete;
      MeDescriptorSetLayout& operator=(const MeDescriptorSetLayout&) = delete;

      VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

   private:
      MeDevice& meDevice;
      VkDescriptorSetLayout descriptorSetLayout;
      std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

      friend class MeDescriptorWriter;
   };

   class MeDescriptorPool {
   public:
      class Builder {
      public:
         Builder(MeDevice& meDevice) : meDevice{ meDevice } {}

         Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
         Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
         Builder& setMaxSets(uint32_t count);
         std::unique_ptr<MeDescriptorPool> build() const;

      private:
         MeDevice& meDevice;
         std::vector<VkDescriptorPoolSize> poolSizes{};
         uint32_t maxSets = 1000;
         VkDescriptorPoolCreateFlags poolFlags = 0;
      };

      MeDescriptorPool(
         MeDevice& meDevice,
         uint32_t maxSets,
         VkDescriptorPoolCreateFlags poolFlags,
         const std::vector<VkDescriptorPoolSize>& poolSizes);
      ~MeDescriptorPool();
      MeDescriptorPool(const MeDescriptorPool&) = delete;
      MeDescriptorPool& operator=(const MeDescriptorPool&) = delete;

      bool allocateDescriptor(
         const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

      void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

      void resetPool();

   private:
      MeDevice& meDevice;
      VkDescriptorPool descriptorPool;

      friend class MeDescriptorWriter;
   };

   class MeDescriptorWriter {
   public:
      MeDescriptorWriter(MeDescriptorSetLayout& setLayout, MeDescriptorPool& pool);

      MeDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
      MeDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

      bool build(VkDescriptorSet& set);
      void overwrite(VkDescriptorSet& set);

   private:
      MeDescriptorSetLayout& setLayout;
      MeDescriptorPool& pool;
      std::vector<VkWriteDescriptorSet> writes;
   };

}  // namespace me
