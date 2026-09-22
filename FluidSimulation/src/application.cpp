#include "application.h"

#include <chrono>

namespace fluid_simulation {

void DestroyGLFWwindow::operator()(GLFWwindow *glfw_window) {
  glfwDestroyWindow(glfw_window);
}

bool createWindow(Window &window, const std::string &title, const int &width, const int &height) {
  std::cout << "[INFO] Initializing GLAD...\n";

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

void runWindow(Window &window, std::function<void(float)> update) {
  std::cout << "[INFO] Starting update loop...\n";

  glfwSwapInterval(1);  // VSync

  using clock = std::chrono::high_resolution_clock;
  using std::chrono::milliseconds, std::chrono::nanoseconds;
  using std::chrono_literals::operator""ms, std::chrono_literals::operator""ns;

  milliseconds timer = 0ms;
  auto curr_time = clock::now(), prev_time = curr_time;
  nanoseconds delta_time = 0ns;
  float dt = 0;
  constexpr long long kOneBillion = 1000000000;
  constexpr int target_ups = 60;
  nanoseconds nano_per_update(kOneBillion / target_ups);
  int frames = 0;
  int updates = 0;

  while (!glfwWindowShouldClose(window.get())) {
    curr_time = clock::now();
    delta_time += std::chrono::duration_cast<nanoseconds>(curr_time - prev_time);
    dt += delta_time.count();
    timer += std::chrono::duration_cast<milliseconds>(curr_time - prev_time);
    prev_time = curr_time;

    if (timer.count() >= 1000) {
      std::cout << "[INFO] FPS: " << frames << ", UPS: " << updates << '\n';
      frames = 0;
      updates = 0;
      timer = 0ms;
    }

    while (delta_time >= nano_per_update) {
      update(dt / kOneBillion);
      dt = 0;
      delta_time -= nano_per_update;
      updates++;
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
