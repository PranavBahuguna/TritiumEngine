#include "Application/FallingSquaresScene.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

void setup(Application *app) {
  // Setup resource paths
  ResourceManager<ShaderCode>::registerFactory(std::make_unique<ShaderLoaderFactory>(),
                                               "Resources/Shaders/");

  // Add window controls callbacks
  app->window.addKeyCallback(Key::ESCAPE, KeyState::PRESSED, [app]() { app->stop(); });
  app->window.setCloseCallback([app]() { app->stop(); });
  app->window.addKeyCallback(Key::R, KeyState::RELEASED,
                             [app]() { app->sceneManager.reloadCurrentScene(); });

  // Add scenes
  app->sceneManager.addScene(std::move(std::make_unique<FallingSquaresScene>()));
}

int main() {
#ifdef _DEBUG
  WindowSettings windowSettings{"main"};
#else
  Logger::Settings::levelMask = LogType::NODEBUG;
  WindowSettings windowSettings{"main", WindowHints::FULLSCREEN | WindowHints::AUTO_MINIMIZE};
#endif // _DEBUG

  try {
    auto *app = new Application("FallingSquares", windowSettings);
    setup(app);
    app->run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  Logger::info("Thank you for playing Falling Squares!");
  return EXIT_SUCCESS;
}