#pragma once

#include <TritiumEngine/Core/Scene.hpp>
#include <TritiumEngine/Rendering/Window.hpp>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Core
{
  class Application;
}

namespace RenderingBenchmark::Scenes
{
  using Application = TritiumEngine::Core::Application;

  class CircleCollisionsScene : public Scene {
  public:
    CircleCollisionsScene(const std::string &name, Application &app);

  protected:
    void init() override;
    void dispose() override;

    CallbackId m_fpsDisplayCallback;
  };
} // namespace RenderingBenchmark::Scenes