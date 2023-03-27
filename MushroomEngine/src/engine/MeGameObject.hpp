#pragma once

#include "MeModel.hpp"
#include "MeBehavior.hpp"
#include "MeCommon.hpp"
#include "MePhysicsModel.hpp"
#include "MePhysicsObject.hpp"

#include <memory>
#include <unordered_map>


namespace me {

class MeGameObject {
public:
   using Map = std::unordered_map<id_t, MeGameObject>;
   static MeGameObject createGameObject() {
      static id_t currentId = 0;
      return MeGameObject{ currentId++ };
   }
   MeGameObject(const MeGameObject&) = delete;
   MeGameObject& operator=(const MeGameObject&) = delete;
   MeGameObject(MeGameObject&&) = default;
   MeGameObject& operator=(MeGameObject &&) = delete;

   id_t getId() { return id; }

   std::shared_ptr<MeModel> model{};
   std::shared_ptr<MePhysicsModel> pModel{};
   std::shared_ptr<MeBehavior> behavior;
   glm::vec3 color{};
   TransformComponent transform{};
   id_t physicsObject = -1;

private:
   MeGameObject(id_t objId) : id(objId) {}
   id_t id;
};

}