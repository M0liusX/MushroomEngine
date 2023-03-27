#pragma once


#include "MeDevice.hpp"
#include "MeModel.hpp"
#include "MePhysicsModel.hpp"
#include "MeObjLoader.hpp"

#include <memory>
#include <unordered_map>

namespace me {
class MeResources {
public:
   MeResources(MeDevice& device) : meDevice(device) {}
   ~MeResources() {}

   MeResources(const MeResources&) = delete;
   MeResources& operator=(const MeResources&) = delete;

   std::shared_ptr<MeModel> getModel(const std::string& filepath) {
      std::unordered_map<std::string, std::shared_ptr<MeModel>>::const_iterator got = models.find(filepath);
      if (got == models.end()) {
         std::shared_ptr<MeModel> model = MeModel::createModelFromFile(meDevice, filepath);
         models.insert({ filepath, model });
         return model;
      }
      else {
         return got->second;
      }
   }

   std::shared_ptr<MePhysicsModel> getPhysicsModel(const std::string& filepath) {
      std::unordered_map<std::string, std::shared_ptr<MePhysicsModel>>::const_iterator got = physicsModels.find(filepath);
      if (got == physicsModels.end()) {
         std::shared_ptr<MePhysicsModel> model = MePhysicsModel::createModelFromFile(meDevice, filepath);
         physicsModels.insert({ filepath, model });
         return model;
      }
      else {
         return got->second;
      }
   }
private:
   MeDevice& meDevice;
   std::unordered_map<std::string, std::shared_ptr<MeModel>> models;
   std::unordered_map<std::string, std::shared_ptr<MePhysicsModel>> physicsModels;
};
}