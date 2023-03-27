#pragma once

#include "MeModel.hpp"
#include "MePhysicsModel.hpp"

#include <vector>

namespace me {
   void loadObj(const std::string& filepath, std::vector<MeModel::Vertex>& vertices, std::vector<uint32_t>& indices);
   void loadPhysicsObj(const std::string& filepath, std::vector<MePhysicsModel::Vertex>& vertices, std::vector<uint32_t>& indices);
}