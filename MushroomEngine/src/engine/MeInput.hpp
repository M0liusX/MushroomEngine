#pragma once

#include <gainput/gainput.h>

#include <cstdio>


class MyDeviceButtonListener : public gainput::InputListener
{
public:
   MyDeviceButtonListener(gainput::InputManager& manager, int index) : manager_(manager), index_(index) { }

   bool OnDeviceButtonBool(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, bool oldValue, bool newValue)
   {
      const gainput::InputDevice* device = manager_.GetDevice(deviceId);
      char buttonName[64] = "";
      device->GetButtonName(deviceButton, buttonName, 64);
      printf("(%d) Device %d (%s%d) bool button (%d/%s) changed: %d -> %d\n", index_, deviceId, device->GetTypeName(), device->GetIndex(), deviceButton, buttonName, oldValue, newValue);
      return false;
   }

   bool OnDeviceButtonFloat(gainput::DeviceId deviceId, gainput::DeviceButtonId deviceButton, float oldValue, float newValue)
   {
      const gainput::InputDevice* device = manager_.GetDevice(deviceId);
      char buttonName[64] = "";
      device->GetButtonName(deviceButton, buttonName, 64);
      printf("(%d) Device %d (%s%d) float button (%d/%s) changed: %f -> %f\n", index_, deviceId, device->GetTypeName(), device->GetIndex(), deviceButton, buttonName, oldValue, newValue);
      return true;
   }

   int GetPriority() const
   {
      return index_;
   }

private:
   gainput::InputManager& manager_;
   int index_;
};

namespace me {
class MeInput {
public:
   enum Button
   {
      WButton,
      AButton,
      SButton,
      DButton,
      QButton,
      EButton,
      StartButton,
      SelectButton,
      UpButton,
      DownButton,
      LeftButton,
      RightButton,
      SpaceButton,
      YStick,
      XStick,
      YStickR,
      XStickR,
      L1Button,
   };
public:
   MeInput(const int WIDTH, const int HEIGHT) {
      manager.SetDisplaySize(WIDTH, HEIGHT);
      const gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();

      map = new gainput::InputMap(manager);
      map->MapBool(WButton, keyboardId, gainput::KeyW);
      map->MapBool(AButton, keyboardId, gainput::KeyA);
      map->MapBool(SButton, keyboardId, gainput::KeyS);
      map->MapBool(DButton, keyboardId, gainput::KeyD);
      map->MapBool(QButton, keyboardId, gainput::KeyQ);
      map->MapBool(EButton, keyboardId, gainput::KeyE);
      map->MapBool(StartButton, keyboardId, gainput::KeyReturn);
      map->MapBool(SelectButton, keyboardId, gainput::KeyShiftR);
      map->MapBool(UpButton, keyboardId, gainput::KeyUp);
      map->MapBool(DownButton, keyboardId, gainput::KeyDown);
      map->MapBool(LeftButton, keyboardId, gainput::KeyLeft);
      map->MapBool(RightButton, keyboardId, gainput::KeyRight);
      map->MapBool(SpaceButton, keyboardId, gainput::KeySpace);

      for (int i = 0; i < 2; i++) {
         const gainput::DeviceId devicePad = manager.CreateDevice<gainput::InputDevicePad>(); // try two pads
         map->MapBool(SpaceButton, devicePad, gainput::PadButtonA);
         map->MapBool(WButton, devicePad, gainput::PadButtonUp);
         map->MapBool(AButton, devicePad, gainput::PadButtonLeft);
         map->MapBool(SButton, devicePad, gainput::PadButtonDown);
         map->MapBool(DButton, devicePad, gainput::PadButtonRight);
         map->MapBool(L1Button, devicePad, gainput::PadButtonL1);
         map->MapBool(SelectButton, devicePad, gainput::PadButtonSelect);
         map->MapFloat(XStick, devicePad, gainput::PadButtonLeftStickX);
         map->MapFloat(YStick, devicePad, gainput::PadButtonLeftStickY);
         map->MapFloat(XStickR, devicePad, gainput::PadButtonRightStickX);
         map->MapFloat(YStickR, devicePad, gainput::PadButtonRightStickY);
      }

      //myDeviceButtonListener = new MyDeviceButtonListener(manager, 1);
      //gainput::ListenerId myDeviceButtonListenerId = manager.AddListener(myDeviceButtonListener);
   }
   ~MeInput() {}

   MeInput(const MeInput&) = delete;
   MeInput& operator=(const MeInput&) = delete;

   void Update() {
      manager.Update();
   }

   gainput::InputMap& getMap() {
      return *map;
   }
   gainput::InputManager* getManager() {
      return &manager;
   }
private:
   gainput::InputManager manager;
   gainput::InputMap* map;
   MyDeviceButtonListener* myDeviceButtonListener;
};
}