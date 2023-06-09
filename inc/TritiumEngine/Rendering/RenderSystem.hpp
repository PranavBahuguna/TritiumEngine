#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <TritiumEngine/Entities/World.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>

using namespace TritiumEngine::Entities;

namespace TritiumEngine::Rendering
{
  class ShaderManager;

  class RenderSystem {
  public:
    RenderSystem(ShaderManager *shaderManager);

    void Draw(const Camera &camera, const World &world) const;
    void DrawInstanced(const Camera &camera, const World &world) const;

  private:
    ShaderManager *m_shaderManager;
  };
} // namespace TritiumEngine::Rendering

#endif // RENDER_SYSTEM_HPP