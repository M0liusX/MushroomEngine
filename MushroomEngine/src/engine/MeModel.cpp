#pragma warning(disable : 26812) // Suppress: The enum type type-name is unscoped. Prefer enum class over enum

#include "MeModel.hpp"
#include "MeObjLoader.hpp"

// std
#include <cassert>
#include <iostream>
#include <unordered_map>

namespace me {


MeModel::MeModel(MeDevice& device, const Builder& builder) :
   meDevice(device)
{
   createVertexBuffers(builder.vertices);
   createIndexBuffer(builder.indices);
}


MeModel::~MeModel() {}


void MeModel::bind(VkCommandBuffer commandBuffer)
{
   VkBuffer buffers[] = { vertexBuffer->getBuffer() };
   VkDeviceSize offsets[] = { 0 };
   vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
   if (hasIndexBuffer) {
      vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
   }
}


void MeModel::draw(VkCommandBuffer commandBuffer)
{
   if (hasIndexBuffer) {
      vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
   }
   else {
      vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
   }
}

std::shared_ptr<MeModel> MeModel::createModelFromFile(MeDevice& device,
                                                      const std::string& filepath)
{
   Builder builder{};
   builder.loadModel(filepath);
   std::cout << filepath << " -> Vertex count: " << builder.vertices.size() << std::endl;
   return std::make_shared<MeModel>(device, builder);
}
void MeModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
   vertexCount = static_cast<uint32_t>(vertices.size());
   assert(vertexCount >= 3 && "Vertex count must be at least 3");
   uint32_t vertexSize = sizeof(vertices[0]);
   VkDeviceSize bufferSize = vertexSize * vertexCount;

   MeBuffer stagingBuffer{
      meDevice,
      vertexSize,
      vertexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
   };

   stagingBuffer.map();
   stagingBuffer.writeToBuffer((void*)vertices.data());

   vertexBuffer = std::make_unique<MeBuffer>(
      meDevice,
      vertexSize,
      vertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
      VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
   );
   meDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
}


void MeModel::createIndexBuffer(const std::vector<uint32_t>& indices)
{
   indexCount = static_cast<uint32_t>(indices.size());
   hasIndexBuffer = indexCount > 0;

   if (!hasIndexBuffer) {
      return;
   }

   assert(indexCount >= 3 && "Index count must be at least 3");
   uint32_t indexSize = sizeof(indices[0]);
   VkDeviceSize bufferSize = indexSize * indexCount;

   MeBuffer stagingBuffer{
      meDevice,
      indexSize,
      indexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
   };

   stagingBuffer.map();
   stagingBuffer.writeToBuffer((void*)indices.data());

   indexBuffer = std::make_unique<MeBuffer>(
      meDevice,
      indexSize,
      indexCount,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
      VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      );
   meDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}


std::vector<VkVertexInputBindingDescription> MeModel::Vertex::getBindingDescription()
{
   std::vector<VkVertexInputBindingDescription> bindingDescriptions{1};
   bindingDescriptions[0].binding = 0;
   bindingDescriptions[0].stride = sizeof(Vertex);
   bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
   return bindingDescriptions;
}


std::vector<VkVertexInputAttributeDescription> MeModel::Vertex::getAttributeDescription()
{
   std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

   /*                               loc bind format                      offset                     */
   attributeDescriptions.push_back({ 0,  0,  VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
   attributeDescriptions.push_back({ 1,  0,  VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
   attributeDescriptions.push_back({ 2,  0,  VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
   attributeDescriptions.push_back({ 3,  0,  VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, texcoord) });
   return attributeDescriptions;
}


void MeModel::Builder::loadModel(const std::string& filepath)
{
   loadObj(filepath, vertices, indices);
}


}  // namespace me