#pragma once

#include "MeDevice.hpp"
#include "MeBuffer.hpp"
#include "MeUtils.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <memory>
#include <vector>
#include <stdint.h>

namespace me {

class MePolygon {
public:

   struct Vertex2D {
      glm::vec2 position{};

      static std::vector<VkVertexInputBindingDescription> getBindingDescription();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

      bool operator==(const Vertex2D& other) const {
         return position == other.position;
      }
   };

   MePolygon(MeDevice &device, const std::vector<Vertex2D>& vertices);
   ~MePolygon();

   MePolygon(const MePolygon&) = delete;
   MePolygon& operator=(const MePolygon&) = delete;

   void bind(VkCommandBuffer commandBuffer);
   void draw(VkCommandBuffer commandBuffer);

private:
   void createVertexBuffers(const std::vector<Vertex2D>& vertices);
   void createIndexBuffer(const std::vector<uint32_t>& indices);

   MeDevice& meDevice;
   std::unique_ptr<MeBuffer> vertexBuffer;
   uint32_t vertexCount;

   bool hasIndexBuffer = false;
   std::unique_ptr<MeBuffer> indexBuffer;
   uint32_t indexCount;
};

}  // namespace me
