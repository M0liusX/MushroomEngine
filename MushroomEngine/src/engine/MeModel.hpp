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

class MeModel {
public:

   struct Vertex {
      glm::vec3 position{};
      glm::vec3 color{};
      glm::vec3 normal{};
      glm::vec2 texcoord{};

      static std::vector<VkVertexInputBindingDescription> getBindingDescription();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

      bool operator==(const Vertex& other) const {
         return position == other.position && color == other.color &&
                normal == other.normal && texcoord == other.texcoord;
      }
   };

   struct Builder {
      std::vector<Vertex> vertices{};
      std::vector<uint32_t> indices{};

      void loadModel(const std::string& filepath);
   };

   MeModel(MeDevice &device, const Builder &builder);
   ~MeModel();

   MeModel(const MeModel&) = delete;
   MeModel& operator=(const MeModel&) = delete;

   static std::shared_ptr<MeModel> createModelFromFile(MeDevice& device,
                                                       const std::string& filepath);

   void bind(VkCommandBuffer commandBuffer);
   void draw(VkCommandBuffer commandBuffer);

private:
   void createVertexBuffers(const std::vector<Vertex> &vertices);
   void createIndexBuffer(const std::vector<uint32_t>& indices);

   MeDevice& meDevice;
   std::unique_ptr<MeBuffer> vertexBuffer;
   uint32_t vertexCount;

   bool hasIndexBuffer = false;
   std::unique_ptr<MeBuffer> indexBuffer;
   uint32_t indexCount;
};

}  // namespace me

namespace std {
   /* Used to load models incompatible with vulkan index buffers. */
   template<>
   struct hash<me::MeModel::Vertex> {
      size_t operator()(me::MeModel::Vertex const& vertex) const {
         size_t seed = 0;
         me::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texcoord);
         return seed;
      }
   };
}  // namespace std
