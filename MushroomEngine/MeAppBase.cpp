#include "MeAppBase.hpp"

/* TODO: Remove Game Dependencies!? */
#include "MeGameBehaviors.hpp"

/* Game Engine */
#include "MeCamera.hpp"
#include "MeResources.hpp"
#include "MeSimpleRenderSystem.hpp"
#include "MePhysicsRenderSystem.hpp"
#include "MeBillboardRenderSystem.hpp"

/* Renderer Aux. */
#include "MeBuffer.hpp"

/* Audio Engine. */
#include "MeAudio.hpp"

/* Graphics Library. */
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

/* Standard Library. */
#include <stdexcept>
#include <cassert>
#include <array>
#include <chrono>
#include <iostream>
#include <numeric>

/* Constants */
const float MAX_FRAME_TIME = 0.5f;


namespace me {


struct GlobalUbo {
   glm::mat4 projection{};
   glm::mat4 view{ 1.f };
   glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity 
   glm::vec3 directionalLight{ 1.f, 1.f, 1.f};
   alignas(16) glm::vec4 directionalLightColor{ 1.f, 1.f, 1.f, .5f };  // w is intensity 
   glm::vec3 pointLightPosition{ 0.f, 0.f, 2.5f };
   alignas(16) glm::vec4 pointLightColor{ 1.f, 0.f, 0.f, .7f };  // w is intensity 
   //alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.f, 3.f, 1.f));
};


MeAppBase::MeAppBase()
{
   start();
}


MeAppBase::~MeAppBase() {}


/* TODO: Move to MeScene! */
id_t MeAppBase::loadGameObject()
{
   auto object = MeGameObject::createGameObject();
   id_t objectId = object.getId();
   gameObjects.emplace(objectId, std::move(object));
   return objectId;
}
void MeAppBase::setModel(id_t objectId, const char* model)
{
   gameObjects.at(objectId).model = meResources.getModel(model);
}
void MeAppBase::setCollider(id_t objectId, id_t colliderId) {
   static const id_t SPHERE = 0;
   static const std::string SPHERE_MODEL = "models/physics/sphere.obj";

   static const id_t PLANE = 1;
   static const std::string PLANE_MODEL = "models/quad.obj";

   static const id_t CUBE = 2;
   static const std::string CUBE_MODEL = "models/cube.obj";

   MeGameObject& object = gameObjects.at(objectId);
   switch (colliderId) {
   case SPHERE:
      object.physicsObject = physicsWorld.AddKinematicObject(MePhysicsObject::createPhysicsObject(object.getId(), object.transform));
      physicsWorld.getObject(object.physicsObject).setCollider(SphereCollider::create(glm::vec3(.0f), 0.5f));
      object.pModel = meResources.getPhysicsModel(SPHERE_MODEL);
      break;
   case PLANE:
      object.physicsObject = physicsWorld.AddStaticObject(MePhysicsObject::createPhysicsObject(object.getId(), object.transform));
      physicsWorld.getObject(object.physicsObject).setCollider(PlaneCollider::create(glm::vec3(0.f, -1.f, 0.f), 0.f));
      object.pModel = meResources.getPhysicsModel(PLANE_MODEL);
      break;
   case CUBE:
      object.physicsObject = physicsWorld.AddStaticObject(MePhysicsObject::createPhysicsObject(object.getId(), object.transform));
      physicsWorld.getObject(object.physicsObject).setCollider(CubeCollider::create(glm::vec3(.0f), glm::vec3(1.f)));
      object.pModel = meResources.getPhysicsModel(CUBE_MODEL);
      break;
   default:
      std::cout << "C++ INVALID COLLIDER" << std::endl;
   }
}
void MeAppBase::setSphereCollider(id_t objectId, float radius, glm::vec3 offset)
{
   static const std::string SPHERE_MODEL = "models/physics/sphere.obj";

   MeGameObject& object = gameObjects.at(objectId);
   object.physicsObject = physicsWorld.AddKinematicObject(MePhysicsObject::createPhysicsObject(object.getId(), object.transform));
   physicsWorld.getObject(object.physicsObject).setCollider(SphereCollider::create(offset, radius));
   object.pModel = meResources.getPhysicsModel(SPHERE_MODEL);
}
void MeAppBase::setPosition(id_t objectId, glm::vec3 position)
{
   gameObjects.at(objectId).transform.translation = position;
}
void MeAppBase::setScale(id_t objectId, glm::vec3 scale)
{
   gameObjects.at(objectId).transform.scale = scale;
}
void MeAppBase::setBehavior(id_t objectId, id_t behaviorId) {
   static const id_t KINEMATIC = 0;
   static const id_t CHARACTER = 1;

   MeGameObject& object = gameObjects.at(objectId);
   switch (behaviorId) {
   case KINEMATIC:
      object.behavior.reset(new MeKinematicBehavior);
      break;
   case CHARACTER:
      object.behavior.reset(new MeKinematicCharacterBehavior);
      break;
   default:
      std::cout << "C++ INVALID BEHAVIOR" << std::endl;
   }
}
/* ------------------------ */


void MeAppBase::start()
{
   const std::string MAIN = "scripts/main.lua";
   meScript.InitScript(MAIN);
}

void MeAppBase::run()
{
   /* Hardware devices. */
   //MeAudio* audio = new MeAudio("sounds/Fearofdark - The Coffee Zone - 15 The Coffee Zone.mp3");

   /* Setup Render Systems. */
   // Buffers
   std::vector<std::unique_ptr<MeBuffer>> uboBuffers(MeSwapChain::MAX_FRAMES_IN_FLIGHT);
   for (int i = 0; i < uboBuffers.size(); i++) {
      uboBuffers[i] = std::make_unique<MeBuffer>(
         meDevice,
         sizeof(GlobalUbo),
         1,
         VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         meDevice.properties.limits.minUniformBufferOffsetAlignment
      );
      uboBuffers[i]->map();
   }
   // Descriptors
   globalDescriptorPool = MeDescriptorPool::Builder(meDevice)
      .setMaxSets(MeSwapChain::MAX_FRAMES_IN_FLIGHT)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MeSwapChain::MAX_FRAMES_IN_FLIGHT)
      .build();
   auto globalDescriptorSetLayout = MeDescriptorSetLayout::Builder(meDevice)
      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
      .build();
   std::vector<VkDescriptorSet> globalDescriptorSets(MeSwapChain::MAX_FRAMES_IN_FLIGHT);
   for (int i = 0; i < globalDescriptorSets.size(); i++) {
      auto bufferInfo = uboBuffers[i]->descriptorInfo();
      MeDescriptorWriter(*globalDescriptorSetLayout, *globalDescriptorPool)
         .writeBuffer(0, &bufferInfo)
         .build(globalDescriptorSets[i]);
   }
   // Render Systems
   MeSimpleRenderSystem simpleRenderSystem{ meDevice,
                                            meRenderer.getSwapChainRenderPass(),
                                            globalDescriptorSetLayout->getDescriptorSetLayout()};
   MePhysicsRenderSystem physicsRenderSystem{ meDevice,
                                              meRenderer.getSwapChainRenderPass(),
                                              globalDescriptorSetLayout->getDescriptorSetLayout() };
   MeBillboardRenderSystem billboardRenderSystem{ meDevice,
                                                  meRenderer.getSwapChainRenderPass(),
                                                  globalDescriptorSetLayout->getDescriptorSetLayout() };

   /* Engine Game Objects. */
   auto pointLight = MeGameObject::createGameObject();
   pointLight.behavior.reset(new MeMovingLight);

   MeCamera camera{};
   auto cameraObj = MeGameObject::createGameObject();
   cameraObj.behavior.reset(new MeBasicCamera);

   /* Game Clock. */
   auto currentTime = std::chrono::high_resolution_clock::now();

   /* Game Loop. */
   while (!meWindow.shouldClose()) {
      /* Reflect window events! */
      float aspect = meRenderer.getAspectRatio();
      meWindow.handleMessages();
      meInput.Update();

      /* Update Clock. */
      auto newTime = std::chrono::high_resolution_clock::now();
      float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
      frameTime = glm::min<float>(frameTime, MAX_FRAME_TIME);  // Optional
      currentTime = newTime;

      /* Perform Behaviors. */
      auto& cameraTransform = cameraObj.transform;
      auto& inputMap = meInput.getMap();
      for (auto& kv : gameObjects) {
         auto& obj = kv.second;
         if (obj.behavior) {
            bool onFloor = obj.physicsObject == -1 ? false : physicsWorld.getObject(obj.physicsObject).collided;
            MeUpdateInfo info = { obj.transform, cameraTransform, inputMap, frameTime, onFloor };
            obj.behavior->update(info);
         }
      }
      MeUpdateInfo pointLightinfo = { pointLight.transform, cameraTransform, inputMap, frameTime, false };
      pointLight.behavior->update(pointLightinfo);
      MeUpdateInfo cameraInfo = { cameraObj.transform, cameraTransform, inputMap, frameTime, false };
      cameraObj.behavior->update(cameraInfo);

      /* Calculate Physics. */
      physicsWorld.CheckCollisions();

      //camera.setOrthographicProjection(-aspect, aspect, -aspect, aspect, -aspect, aspect);
      camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 1000);
      camera.setViewTarget(gameObjects.at(0).transform.translation + cameraObj.transform.translation, gameObjects.at(0).transform.translation, glm::vec3(0.f, -1.f, 0.f));
      //camera.setViewYXZ(cameraObj.transform.translation, cameraObj.transform.rotation);

      /* Render System. */
      if (auto commandBuffer = meRenderer.beginFrame()) {
         int frameIndex = meRenderer.getFrameIndex();
         FrameInfo frameInfo{
            frameIndex,
            frameTime,
            commandBuffer,
            camera,
            globalDescriptorSets[frameIndex],
            gameObjects,
            physicsWorld,
         };

         /* Update Memory. */
         GlobalUbo ubo{};
         ubo.projection = camera.getProjection();
         ubo.view = camera.getView();
         ubo.pointLightPosition = glm::vec3(pointLight.transform.translation);
         uboBuffers[frameIndex]->writeToBuffer(&ubo);
         uboBuffers[frameIndex]->flush();

         /* Draw Calls. */
         meRenderer.beginSwapChainRenderPass(commandBuffer);
         if(showPhysics) physicsRenderSystem.renderPhysicsObjects(frameInfo);
         simpleRenderSystem.renderGameObjects(frameInfo);
         billboardRenderSystem.renderGameObjects(frameInfo);
         meRenderer.endSwapChainRenderPass(commandBuffer);
         meRenderer.endFrame();
      }
   }

   vkDeviceWaitIdle(meDevice.device());
}


}  // namespace me
