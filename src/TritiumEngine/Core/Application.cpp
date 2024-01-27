#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Window.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Core
{
  Application::Application(const std::string &name, const WindowSettings &settings)
      : name(name), window(settings), sceneManager(this) {
    initGLEW();
  }

  /** @brief Starts running the application */
  void Application::run() {
    if (!sceneManager.hasScenes()) {
      Logger::error("[Application] App '{}' has no registered scenes!", name);
      throw std::runtime_error("An error ocurred while running the application.");
    }

    if (m_isRunning) {
      Logger::warn("[Application] App '{}' is already running!", name);
      return;
    }

    m_isRunning = true;
    Logger::info("[Application] App '{}' running...", name);
    sceneManager.loadScene(0);
    m_prevFrameTime = Clock::now();

    // Run main application loop
    while (m_isRunning) {
      // Update time
      m_currentTime   = Clock::now();
      float deltaTime = std::chrono::duration<float>(m_currentTime - m_prevFrameTime).count();
      m_prevFrameTime = m_currentTime;

      // Update window
      window.update();
      window.refresh();

      Logger::info("FPS = {}", 1.0f / deltaTime);

      // Update scene
      sceneManager.update(deltaTime);
    }

    Logger::info("[Application] App '{}' stopped.", name);
  }

  /** @brief Sets application flagged to stop */
  void Application::stop() {
    m_isRunning = false;
    Logger::info("[Application] Stopping app '{}'...", name);
  }

  /** @brief Check if the application is currently running */
  bool Application::isRunning() const { return m_isRunning; }

  void Application::initGLEW() const {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
      throw std::runtime_error("GLEW could not be initialised!");
  }
} // namespace TritiumEngine::Core