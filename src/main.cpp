#include "application.h"

int main(int argc, char *argv[]) {
  int quit_timeout = 5 * 60;
  int min_particles = 0;
  int max_particles = 1000;

  if (argc > 1) {
    quit_timeout = std::stoi(argv[1]);
  }

  if (argc > 2) {
    min_particles = std::stoi(argv[2]);
  }

  if (argc > 3) {
    max_particles = std::stoi(argv[3]);
  }

  Application app(quit_timeout, min_particles, max_particles);

  app.Init();
  app.Run();

  return 0;
}
