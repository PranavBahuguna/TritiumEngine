#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Rendering/Camera.hpp>
#include <TritiumEngine/Rendering/InstancedRenderable.hpp>
#include <TritiumEngine/Rendering/Renderable.hpp>
#include <TritiumEngine/Rendering/Shader.hpp>
#include <TritiumEngine/Rendering/StandardRenderSystem.hpp>
#include <TritiumEngine/Utilities/ColorUtils.hpp>

namespace TritiumEngine::Rendering
{
  StandardRenderSystem::StandardRenderSystem(Tag cameraTag, BlendOptions blendOptions)
      : RenderSystem(cameraTag, blendOptions) {}

  void StandardRenderSystem::draw(const Camera &camera) const {
    drawNonInstanced(camera);
    drawInstanced(camera);
  }

  void StandardRenderSystem::drawNonInstanced(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<Renderable, Transform, Shader, Color>().each(
        [&](auto entity, Renderable &renderable, Transform &transform, Shader &shader,
            Color &color) {
          // Apply properties to shader
          if (shader.id != shaderManager.getCurrentShader()) {
            shaderManager.use(shader.id);
            shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
          }
          shaderManager.setMatrix4("model", transform.getModelMatrix());
          shaderManager.setVector4("color", ColorUtils::ToNormalizedVec4(color));

          unsigned int vao        = renderable.getVao();
          int vertexStride        = renderable.getVertexStride();
          int nVertices           = renderable.getNumVertices();
          int nIndices            = renderable.getNumIndices();
          unsigned int renderMode = renderable.getRenderMode();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElements(renderMode, nIndices, GL_UNSIGNED_INT, 0);
          else
            glDrawArrays(renderMode, 0, nVertices / vertexStride);
        });
  }

  void StandardRenderSystem::drawInstanced(const Camera &camera) const {
    ShaderManager &shaderManager = m_app->shaderManager;

    m_app->registry.view<InstancedRenderable, Shader>().each(
        [&](auto entity, InstancedRenderable &renderable, Shader &shader) {
          unsigned int vao        = renderable.getVao();
          int vertexStride        = renderable.getVertexStride();
          int nVertices           = renderable.getNumVertices();
          int nIndices            = renderable.getNumIndices();
          int nInstances          = renderable.getNumInstances();
          unsigned int renderMode = renderable.getRenderMode();
          uint32_t instanceId     = renderable.getInstanceId();

          if (shader.id != shaderManager.getCurrentShader()) {
            shaderManager.use(shader.id);
            shaderManager.setMatrix4("projectionView", camera.calcProjectionViewMatrix());
          }

          // Update model matrices
          int index          = 0;
          auto instancesView = m_app->registry.view<Transform, InstancedRenderableTag>();
          m_app->registry.view<Transform, Color, InstancedRenderableTag>().each(
              [&](auto entity, Transform &transform, Color &color, InstancedRenderableTag &tag) {
                if (index < nInstances && tag.value == instanceId)
                  renderable.setInstanceData(index++, {transform.getModelMatrix(), color.value});
              });
          renderable.updateInstanceDataBuffer();

          // Draw the renderable
          glBindVertexArray(vao);
          if (nIndices > 0)
            glDrawElementsInstanced(renderMode, nIndices, GL_UNSIGNED_INT, 0, nInstances);
          else
            glDrawArraysInstanced(renderMode, 0, nVertices / vertexStride, nInstances);
        });
  }
} // namespace TritiumEngine::Rendering