#include "MePhysicsServer.hpp"


namespace me {


void MePhysicsServer::CheckCollisions()
{
   static const glm::vec3 DOWN(0.f, 1.f, 0.f);
   for (id_t k : kinematic) {
      MePhysicsObject& objk = physicsObjects.at(k);
      objk.collided = false;
      for (id_t s : fixed) {
         MePhysicsObject& objs = physicsObjects.at(s);
         //if (obja.getId() == objb.getId()) break;
         if (!objk.getCollider() || !objs.getCollider()) {
            continue;
         }
         CollisionPoints p = objk.getCollider()->TestCollision(objk.getTransform(), objs.getCollider(), objs.getTransform());
         /* Solve Collision */
         if (p.hasCollision) {
            glm::vec3 offsetInside = p.b - p.a;
            float angle = glm::angle(glm::normalize(p.a - objk.getTransform().translation), DOWN);
            objk.collided |= angle < 0.78539816339 ? true : false;
            objk.getTransform().translation += offsetInside;
         }
         //objk.collided |= p.hasCollision;
         objs.collided |= p.hasCollision;
      }
   }
}


}