#include "application.h"

int main() {
  fluid_simulation::Window window;
  if (!fluid_simulation::createWindow(window, "Fluid Simulation", 800, 800)) return -1;
  fluid_simulation::runWindow(window); 
}
