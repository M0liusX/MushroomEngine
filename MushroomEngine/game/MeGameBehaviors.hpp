#pragma once

#include "MeBehavior.hpp"
#include "MeAudio.hpp"

//#include <glm/gtx/vector_angle.hpp>

#include <cmath>
#define PI 3.14159265358979323846f
#define PI_2 1.57079632679489661923f

float g_angle = 0;

namespace me {

struct MeKinematicCharacterBehavior : MeBehavior
{
   const float JUMP = -3.0f;
   float yvel = 0.0f;

   const int JUMP_BUFFER_FRAMES = 20;
   int bufferFrames = 5;

   void ready() override
   {}

   void update(MeUpdateInfo& info) override
   {

      if (!info.collided) {
         yvel += info.delta * 9.8f;
         bufferFrames = std::max<int>(bufferFrames - 1, 0);
         if (info.map.GetBoolIsNew(MeInput::SpaceButton) &&
             yvel >= 0.f &&
             bufferFrames > 0) {
            yvel = JUMP;
         }
      }
      else {
         bufferFrames = JUMP_BUFFER_FRAMES;
         if (info.map.GetBoolIsNew(MeInput::SpaceButton)) {
            yvel = JUMP;
         }
         yvel = std::min<float>(yvel, 0.f);
      }
      float yaw = info.camera.rotation.y;
      glm::vec3 forward{ sin(yaw), 0.f, cos(yaw) };
      glm::vec3 right({ forward.z, 0.f, -forward.x });

      ///* Digital Input */
      //glm::vec3 move{ 0.f };
      //if (info.map.GetBool(MeInput::RightButton)) move += right;
      //if (info.map.GetBool(MeInput::LeftButton)) move -= right;
      //if (info.map.GetBool(MeInput::UpButton)) move += forward;
      //if (info.map.GetBool(MeInput::DownButton)) move -= forward;

      /* Analouge Input */
      float xaxis = info.map.GetFloat(MeInput::XStick);
      float yaxis = info.map.GetFloat(MeInput::YStick);
      glm::vec3 move = right * xaxis + forward * yaxis;
      if (glm::dot(move, move) > std::numeric_limits<float>::epsilon()) {
         info.t.translation += 2.f * info.delta * move;
         //g_angle = atan2(move.x, move.z);
         info.t.rotation.y = atan2(move.x, move.z);
         g_angle = info.t.rotation.y;
      }
      info.t.translation.y += info.delta * yvel;
   }
};

struct MeKinematicBehavior : MeBehavior
{
   void ready() override
   {}

   void update(MeUpdateInfo& info) override
   {
      if (!info.collided) {
         info.t.translation.y += info.delta * 3.f;
      }
   }
};

struct MeMovingLight : MeBehavior
{
   float totalTime = 0.0f;
   void ready() override
   {}

   void update(MeUpdateInfo& info) override
   {
      totalTime += info.delta;
      info.t.translation.z = 0.5f * sin(totalTime * 2.f) + 2.5f;
      info.t.translation.x = 0.5f * cos(totalTime * 2.f);
      info.t.translation.y = -.25f;
   }
};

struct MeKinematicCamera : me::MeBehavior
{
   MeAudio* audio = new MeAudio("sounds/Fearofdark - The Coffee Zone - 15 The Coffee Zone.mp3");
   MeAudio* sfx = new MeAudio("sounds/sfx/fast_snare.wav");
   void ready() override
   {
      audio->Start();
   }

   void update(MeUpdateInfo& info) override
   {
      glm::vec3 move{0.f};
      glm::vec3 rotate{0.f};

      float yaw = info.t.rotation.y;
      glm::vec3 forward{ sin(yaw), 0.f, cos(yaw) };
      glm::vec3 right({ forward.z, 0.f, -forward.x });
      glm::vec3 up({ 0.f, -1.f, 0.f });

      if (info.map.GetBoolIsNew(MeInput::StartButton)) {
         sfx->Start();
      }
      if (info.map.GetBoolIsNew(MeInput::SelectButton)) {
         audio->Start();
      }
      if (info.map.GetBool(MeInput::DButton)) move += right;
      if (info.map.GetBool(MeInput::AButton)) move -= right;
      if (info.map.GetBool(MeInput::WButton)) move += forward;
      if (info.map.GetBool(MeInput::SButton)) move -= forward;
      if (info.map.GetBool(MeInput::QButton)) move -= up;
      if (info.map.GetBool(MeInput::EButton)) move += up;
      if (info.map.GetBool(MeInput::RightButton)) rotate.y += 1.f;
      if (info.map.GetBool(MeInput::LeftButton)) rotate.y -= 1.f;
      if (info.map.GetBool(MeInput::UpButton)) rotate.x += 1.f;
      if (info.map.GetBool(MeInput::DownButton)) rotate.x -= 1.f;

      if (glm::dot(move, move) > std::numeric_limits<float>::epsilon()) {
         info.t.translation += 3.f * info.delta * glm::normalize(move);
      }
      if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
         info.t.rotation += 2.f * info.delta * glm::normalize(rotate);
      }
   }
};


struct MeBasicCamera : me::MeBehavior
{
   MeAudio* audio = new MeAudio("sounds/Fearofdark - The Coffee Zone - 15 The Coffee Zone.mp3");
   MeAudio* sfx = new MeAudio("sounds/sfx/fast_snare.wav");
   const float RADIUS = 1.f;
   const float SPEED = 2.f;
   float theta = PI_2 * 3;
   float y = .5;

   void ready() override
   {}

   void update(MeUpdateInfo& info) override
   {
      if (info.map.GetBoolIsNew(MeInput::SpaceButton)) {
         sfx->Start();
      }
      if (info.map.GetBoolIsNew(MeInput::SelectButton)) {
         audio->Start();
      }

      if (info.map.GetBoolIsNew(MeInput::L1Button)) {
         theta = -(g_angle + PI_2);
      }

      theta -= info.map.GetFloat(MeInput::XStickR) * info.delta * SPEED;
      theta = theta > (2.f * PI) ? theta - 2.f * PI : theta;
      theta = theta < 0 ? theta + 2.f * PI : theta;
      y += info.map.GetFloat(MeInput::YStickR) * info.delta * SPEED * (1.f / PI);
      y = std::min<float>(std::max<float>(y, -1.f), 0.f);
      info.t.translation = { cos(theta), y, sin(theta) };
      info.t.rotation.y = -theta - PI_2;
   }
};


}