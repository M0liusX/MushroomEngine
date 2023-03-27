#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/* Input Library. */
#include <gainput/gainput.h>
#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "ws2_32.lib")

#include <string>

namespace me {

class MeWindow {
public:
   MeWindow(int w, int h, std::string name, gainput::InputManager* inputManager);
   ~MeWindow();

   MeWindow(const MeWindow &) = delete;
   MeWindow &operator=(const MeWindow &) = delete;

   bool shouldClose() { return glfwWindowShouldClose(window); }
   VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
   bool wasWindowResized() { return framebufferResized; }
   void resetWindowResizedFlag() { framebufferResized = false; }

   void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
   void handleMessages();
private:
   static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
   void initWindow();

   int width;
   int height;
   bool framebufferResized = false;

   gainput::InputManager* inputManager_;
   std::string windowName;
   GLFWwindow *window;
};

}  // namespace me
