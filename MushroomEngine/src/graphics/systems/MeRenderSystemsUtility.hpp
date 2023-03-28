#pragma once

#include "MeCamera.hpp"
#include "MeDevice.hpp"
#include "MeDescriptors.hpp"
#include "MeGameObject.hpp"
#include "MePhysicsServer.hpp"
#include "MePipeline.hpp"

namespace me {

struct FrameInfo {
   int frameIndex;
   float frameTime;
   VkCommandBuffer commandBuffer;
   MeCamera& camera;
   VkDescriptorSet desciptorSet;
   MeDescriptorPool& frameDescriptorPool;
   MeGameObject::Map& gameObjects;
   MePhysicsServer& physicsServer;
};

}