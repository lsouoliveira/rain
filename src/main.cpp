#include "application.h"

int main(int argc, char *argv[]) {
  int quit_timeout = 5 * 60;

  if (argc > 1) {
    quit_timeout = std::stoi(argv[1]);
  }

  Application app(quit_timeout);

  app.Init();
  app.Run();

  return 0;
}
