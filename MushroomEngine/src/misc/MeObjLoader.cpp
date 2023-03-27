#include "MeObjLoader.hpp"

// libs
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


namespace me {


void loadObj(const std::string& filepath,
             std::vector<MeModel::Vertex>& vertices,
             std::vector<uint32_t>& indices)
{
   tinyobj::attrib_t attrib;
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;
   std::string warn, err;

   if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
      throw std::runtime_error(warn + err);
   }

   vertices.clear();
   indices.clear();

   std::unordered_map<MeModel::Vertex, uint32_t> uniqueVertices{};
   for (const auto& shape : shapes) {
      for (const auto& index : shape.mesh.indices) {
         MeModel::Vertex vertex{};
         if (index.vertex_index >= 0) {
            vertex.position = {
               attrib.vertices[3 * index.vertex_index + 0],
               attrib.vertices[3 * index.vertex_index + 1],
               attrib.vertices[3 * index.vertex_index + 2],
            };
            vertex.color = {
               attrib.colors[3 * index.vertex_index + 0],
               attrib.colors[3 * index.vertex_index + 1],
               attrib.colors[3 * index.vertex_index + 2],
            };
         }
         if (index.normal_index >= 0) {
            vertex.normal = {
               attrib.normals[3 * index.normal_index + 0],
               attrib.normals[3 * index.normal_index + 1],
               attrib.normals[3 * index.normal_index + 2],
            };
         }
         if (index.texcoord_index >= 0) {
            vertex.texcoord = {
               attrib.texcoords[2 * index.texcoord_index + 0],
               attrib.texcoords[2 * index.texcoord_index + 1],
            };
         }

         if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
         }
         indices.push_back(uniqueVertices[vertex]);
      }
   }
}


void loadPhysicsObj(const std::string& filepath,
                    std::vector<MePhysicsModel::Vertex>& vertices,
                    std::vector<uint32_t>& indices)
{
   tinyobj::attrib_t attrib;
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;
   std::string warn, err;

   if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
      throw std::runtime_error(warn + err);
   }

   vertices.clear();
   indices.clear();

   std::unordered_map<MePhysicsModel::Vertex, uint32_t> uniqueVertices{};
   for (const auto& shape : shapes) {
      for (const auto& index : shape.mesh.indices) {
         MePhysicsModel::Vertex vertex{};
         if (index.vertex_index >= 0) {
            vertex.position = {
               attrib.vertices[3 * index.vertex_index + 0],
               attrib.vertices[3 * index.vertex_index + 1],
               attrib.vertices[3 * index.vertex_index + 2],
            };
            vertex.color = {
               attrib.colors[3 * index.vertex_index + 0],
               attrib.colors[3 * index.vertex_index + 1],
               attrib.colors[3 * index.vertex_index + 2],
            };
         }

         if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);
         }
         indices.push_back(uniqueVertices[vertex]);
      }
   }
}


}
