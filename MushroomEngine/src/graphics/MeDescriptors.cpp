#pragma warning(disable : 26812) // Suppress: The enum type type-name is unscoped. Prefer enum class over enum

#include "MeDescriptors.hpp"

// std
#include <cassert>
#include <stdexcept>


namespace me {


// *************** Descriptor Set Layout Builder *********************

MeDescriptorSetLayout::Builder& MeDescriptorSetLayout::Builder::addBinding(uint32_t binding,
                                                                           VkDescriptorType descriptorType,
                                                                           VkShaderStageFlags stageFlags,
                                                                           uint32_t count)
{
   assert(bindings.count(binding) == 0 && "Binding already in use");
   VkDescriptorSetLayoutBinding layoutBinding{};
   layoutBinding.binding = binding;
   layoutBinding.descriptorType = descriptorType;
   layoutBinding.descriptorCount = count;
   layoutBinding.stageFlags = stageFlags;
   bindings[binding] = layoutBinding;
   return *this;
}

std::unique_ptr<MeDescriptorSetLayout> MeDescriptorSetLayout::Builder::build() const
{
   return std::make_unique<MeDescriptorSetLayout>(meDevice, bindings);
}


// *************** Descriptor Set Layout *********************

MeDescriptorSetLayout::MeDescriptorSetLayout(MeDevice& meDevice,
                                             std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) :
   meDevice{ meDevice }, bindings{ bindings }
{
   std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
   for (auto kv : bindings) {
      setLayoutBindings.push_back(kv.second);
   }

   VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
   descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
   descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
   descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

   if (vkCreateDescriptorSetLayout(
      meDevice.device(),
      &descriptorSetLayoutInfo,
      nullptr,
      &descriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
   }
}


MeDescriptorSetLayout::~MeDescriptorSetLayout()
{
   vkDestroyDescriptorSetLayout(meDevice.device(), descriptorSetLayout, nullptr);
}


// *************** Descriptor Pool Builder *********************

MeDescriptorPool::Builder& MeDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType,
                                                                  uint32_t count)
{
   poolSizes.push_back({ descriptorType, count });
   return *this;
}


MeDescriptorPool::Builder& MeDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
{
   poolFlags = flags;
   return *this;
}


MeDescriptorPool::Builder& MeDescriptorPool::Builder::setMaxSets(uint32_t count)
{
   maxSets = count;
   return *this;
}


std::unique_ptr<MeDescriptorPool> MeDescriptorPool::Builder::build() const
{
   return std::make_unique<MeDescriptorPool>(meDevice, maxSets, poolFlags, poolSizes);
}


// *************** Descriptor Pool *********************

MeDescriptorPool::MeDescriptorPool(MeDevice& meDevice,
                                   uint32_t maxSets,
                                   VkDescriptorPoolCreateFlags poolFlags,
                                   const std::vector<VkDescriptorPoolSize>& poolSizes) :
   meDevice{ meDevice }
{
   VkDescriptorPoolCreateInfo descriptorPoolInfo{};
   descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
   descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
   descriptorPoolInfo.pPoolSizes = poolSizes.data();
   descriptorPoolInfo.maxSets = maxSets;
   descriptorPoolInfo.flags = poolFlags;

   if (vkCreateDescriptorPool(meDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
   }
}


MeDescriptorPool::~MeDescriptorPool()
{
   vkDestroyDescriptorPool(meDevice.device(), descriptorPool, nullptr);
}


bool MeDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                                          VkDescriptorSet& descriptor) const
{
   VkDescriptorSetAllocateInfo allocInfo{};
   allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
   allocInfo.descriptorPool = descriptorPool;
   allocInfo.pSetLayouts = &descriptorSetLayout;
   allocInfo.descriptorSetCount = 1;

   // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
   // a new pool whenever an old pool fills up. But this is beyond our current scope
   if (vkAllocateDescriptorSets(meDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
      return false;
   }
   return true;
}


void MeDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
{
   vkFreeDescriptorSets(meDevice.device(),
                        descriptorPool,
                        static_cast<uint32_t>(descriptors.size()),
                        descriptors.data());
}


void MeDescriptorPool::resetPool()
{
   vkResetDescriptorPool(meDevice.device(), descriptorPool, 0);
}


// *************** Descriptor Writer *********************
MeDescriptorWriter::MeDescriptorWriter(MeDescriptorSetLayout& setLayout,
                                       MeDescriptorPool& pool) :
   setLayout{ setLayout }, pool{ pool }
{}


MeDescriptorWriter& MeDescriptorWriter::writeBuffer(uint32_t binding,
                                                    VkDescriptorBufferInfo* bufferInfo)
{
   assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

   auto& bindingDescription = setLayout.bindings[binding];

   assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

   VkWriteDescriptorSet write{};
   write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
   write.descriptorType = bindingDescription.descriptorType;
   write.dstBinding = binding;
   write.pBufferInfo = bufferInfo;
   write.descriptorCount = 1;

   writes.push_back(write);
   return *this;
}


MeDescriptorWriter& MeDescriptorWriter::writeImage(uint32_t binding,
                                                   VkDescriptorImageInfo* imageInfo)
{
   assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

   auto& bindingDescription = setLayout.bindings[binding];

   assert(
      bindingDescription.descriptorCount == 1 &&
      "Binding single descriptor info, but binding expects multiple");

   VkWriteDescriptorSet write{};
   write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
   write.descriptorType = bindingDescription.descriptorType;
   write.dstBinding = binding;
   write.pImageInfo = imageInfo;
   write.descriptorCount = 1;

   writes.push_back(write);
   return *this;
}


bool MeDescriptorWriter::build(VkDescriptorSet& set)
{
   bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
   if (!success) {
      return false;
   }
   overwrite(set);
   return true;
}


void MeDescriptorWriter::overwrite(VkDescriptorSet& set)
{
   for (auto& write : writes) {
      write.dstSet = set;
   }
   vkUpdateDescriptorSets(pool.meDevice.device(), writes.size(), writes.data(), 0, nullptr);
}


}  // namespace me