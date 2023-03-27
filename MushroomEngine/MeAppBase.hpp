#pragma once

#include "MeWindow.hpp"
#include "MeDevice.hpp"
#include "MeInput.hpp"
#include "MePipeline.hpp"
#include "MeRenderer.hpp"
#include "MeResources.hpp"
#include "MeGameObject.hpp"
#include "MePhysicsServer.hpp"
#include "MeDescriptors.hpp"
#include "MeScript.hpp"

#include <memory>

namespace me {

class MeAppBase {
public:
   static constexpr int WIDTH = 960;
   static constexpr int HEIGHT = 540;

   MeAppBase();
   ~MeAppBase();

   MeAppBase(const MeAppBase&) = delete;
   MeAppBase& operator=(const MeAppBase&) = delete;

   void run();

   void setShowPhysics() {
      showPhysics = true;
   }

   /* TODO: MOVE */
   id_t loadGameObject();
   void setModel(id_t objectId, const char* model);
   void setCollider(id_t objectId, id_t colliderId);
   void setSphereCollider(id_t objectId, float radius, glm::vec3 offset);
   void setBehavior(id_t objectId, id_t behaviorId);
   void setPosition(id_t objectId, glm::vec3 position);
   void setScale(id_t objectId, glm::vec3 scale);
private:
   void start();

   MeInput meInput{ WIDTH, HEIGHT };
   MeWindow meWindow{ WIDTH, HEIGHT, "Hello Vulkan!", meInput.getManager()};
   MeDevice meDevice{ meWindow };
   MeRenderer meRenderer{ meWindow, meDevice };
   bool showPhysics = false;

   /* XXX: Destroy pool before device! */
   std::unique_ptr<MeDescriptorPool> globalDescriptorPool{};
   MeResources meResources{ meDevice };
   MeGameObject::Map gameObjects;
   MePhysicsServer physicsWorld;
   MeScript meScript{ static_cast<void*>(this) };
};

}  // namespace me
