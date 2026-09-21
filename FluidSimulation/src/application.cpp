#include "application.h"

#include <chrono>

namespace fluid_simulation {

void DestroyGLFWwindow::operator()(GLFWwindow *glfw_window) {
  glfwDestroyWindow(glfw_window);
}

bool createWindow(Window &window, const std::string &title, const int &width, const int &height) {
  std::cout << "[INFO] Initializing GLFW...\n";

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = std::unique_ptr<GLFWwindow, DestroyGLFWwindow>(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
  if (!window.get()) {
    std::cout << "[ERROR] Failed to create GLFW window!\n";
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window.get());

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "[ERROR] Failed to initialize GLAD!\n";
    return false;
  }

  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    });

  glfwFocusWindow(window.get());

  return true;
}

void runWindow(Window &window) {
  std::cout << "[INFO] Starting update loop...\n";

  glfwSwapInterval(1);  // VSync

  using clock = std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;
  using std::chrono_literals::operator""ms;

  milliseconds timer = 0ms;
  auto curr_time = clock::now(), prev_time = curr_time;
  long long frames = 0;

  while (!glfwWindowShouldClose(window.get())) {
    curr_time = clock::now();
    timer += std::chrono::duration_cast<milliseconds>(curr_time - prev_time);
    prev_time = curr_time;
    while (timer.count() >= 1000) {
      std::cout << "[INFO] FPS: " << frames << '\n';
      frames = 0;
      timer = 0ms;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window.get());
    frames++;

    glfwPollEvents();
  }

  glfwTerminate();
}

}
