#pragma once

#include "MePhysicsColliders.hpp"
#include "MeCommon.hpp"

#include <unordered_map>
#include <vector>

namespace me {
class MePhysicsObject {
public:
   using Map = std::unordered_map<id_t, MePhysicsObject>;
   using Layer = std::vector<id_t>;
   static MePhysicsObject createPhysicsObject(id_t gameObject, TransformComponent& t) {
      return MePhysicsObject( gameObject, t );
   }
   MePhysicsObject(const MePhysicsObject&) = delete;
   MePhysicsObject& operator=(const MePhysicsObject&) = delete;
   MePhysicsObject(MePhysicsObject&&) = default;
   MePhysicsObject& operator=(MePhysicsObject&&) = delete;

   id_t getOwner() { return owner; }
   void setCollider(Collider* c) { collider = c; }
   Collider* getCollider() { return collider; }
   TransformComponent& getTransform() { return t; }
   bool collided = false;
private:
   MePhysicsObject(id_t id, TransformComponent& t) : owner(id), t(t) {}
   id_t owner;
   TransformComponent& t;
   Collider* collider;
};
}