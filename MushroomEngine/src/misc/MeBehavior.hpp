#pragma once

#include "MeCommon.hpp"
#include "MeInput.hpp"

namespace me {

   struct MeUpdateInfo {
      TransformComponent& t;
      TransformComponent& camera;
      gainput::InputMap& map;
      float delta;
      bool collided;
   };

   struct MeBehavior {
      virtual void ready() = 0;
      virtual void update(MeUpdateInfo& info) = 0;
   };
}