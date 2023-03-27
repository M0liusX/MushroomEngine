#include "MeWindow.hpp"

#if _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <stdexcept>

namespace me {


MeWindow::MeWindow(int w, int h, 
                   std::string name, 
                   gainput::InputManager* inputManager) :
   width{ w }, height{ h }, windowName{ name }, inputManager_{ inputManager }
{
  initWindow();
}


MeWindow::~MeWindow()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}


void MeWindow::initWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void MeWindow::handleMessages()
{
#if _WIN32
   {
      MSG msg;
      HWND hWnd = glfwGetWin32Window(window);// static_cast<HWND>(s_handle.native);
      while (PeekMessageW(&msg, hWnd, 0, 0, PM_REMOVE)) {
         TranslateMessage(&msg);
         //DispatchMessageW will trigger glfwResizeCallback so we don't have to call glfwPollEvents
         DispatchMessageW(&msg);

         // Forward any input messages to Gainput
         inputManager_->HandleMessage(msg);
      }
   }
#endif
}

void MeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
   if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("failed to create window surface");
   }
}


void MeWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
   auto meWindow = reinterpret_cast<MeWindow*>(glfwGetWindowUserPointer(window));
   meWindow->framebufferResized = true;
   meWindow->width = width;
   meWindow->height = height;
}


}  // namespace me
