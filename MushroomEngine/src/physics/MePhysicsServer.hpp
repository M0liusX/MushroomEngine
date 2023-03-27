#pragma once

#include "MePhysicsObject.hpp"

namespace me {
class MePhysicsServer {
public:
   id_t AddKinematicObject(MePhysicsObject&& pObject) {
      physicsObjects.emplace(currentId, std::move(pObject));
      kinematic.push_back(currentId);
      return currentId++;
   }
   id_t AddStaticObject(MePhysicsObject&& pObject) {
      physicsObjects.emplace(currentId, std::move(pObject));
      fixed.push_back(currentId);
      return currentId++;
   }
   MePhysicsServer() {}
   MePhysicsServer(MePhysicsServer&&) = default;
   MePhysicsServer& operator=(MePhysicsServer&&) = delete;

   MePhysicsObject& getObject(id_t id) { return physicsObjects.at(id); }
   void CheckCollisions();
   //void SolveCollision()
private:
   int currentId = 0;
   MePhysicsObject::Map physicsObjects;
   MePhysicsObject::Layer kinematic;
   MePhysicsObject::Layer fixed;
};
}