#pragma warning(disable : 26812) // Suppress: The enum type type-name is unscoped. Prefer enum class over enum

#include "MePolygon.hpp"
#include "MeObjLoader.hpp"

// std
#include <cassert>
#include <iostream>
#include <unordered_map>

namespace me {


   MePolygon::MePolygon(MeDevice& device, const std::vector<Vertex2D>& vertices) :
   meDevice(device)
{
   createVertexBuffers(vertices);
   //createIndexBuffer(builder.indices);
}


   MePolygon::~MePolygon() {}


void MePolygon::bind(VkCommandBuffer commandBuffer)
{
   VkBuffer buffers[] = { vertexBuffer->getBuffer() };
   VkDeviceSize offsets[] = { 0 };
   vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
   if (hasIndexBuffer) {
      vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
   }
}


void MePolygon::draw(VkCommandBuffer commandBuffer)
{
   if (hasIndexBuffer) {
      vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
   }
   else {
      vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
   }
}

void MePolygon::createVertexBuffers(const std::vector<Vertex2D>& vertices)
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


void MePolygon::createIndexBuffer(const std::vector<uint32_t>& indices)
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


std::vector<VkVertexInputBindingDescription> MePolygon::Vertex2D::getBindingDescription()
{
   std::vector<VkVertexInputBindingDescription> bindingDescriptions{1};
   bindingDescriptions[0].binding = 0;
   bindingDescriptions[0].stride = sizeof(Vertex2D);
   bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
   return bindingDescriptions;
}


std::vector<VkVertexInputAttributeDescription> MePolygon::Vertex2D::getAttributeDescription()
{
   std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

   /*                               loc bind format                      offset                     */
   attributeDescriptions.push_back({ 0,  0,  VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex2D, position)});
   return attributeDescriptions;
}

}  // namespace me