#pragma once

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <memory>

namespace fluid_simulation {

struct DestroyGLFWwindow {
  void operator()(GLFWwindow *glfw_window);
};

typedef std::unique_ptr<GLFWwindow, DestroyGLFWwindow> Window;

bool createWindow(Window &window, const std::string &title, const int &width, const int &height);

void runWindow(Window &window);

}

