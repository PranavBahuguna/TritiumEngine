#pragma once

#include "Scenes/CubeScene.hpp"
#include "Components/Tags.hpp"
#include "Systems/CubeRenderSystem.hpp"

#include <TritiumEngine/Core/Application.hpp>
#include <TritiumEngine/Core/Components/NativeScript.hpp>
#include <TritiumEngine/Rendering/Primitives.hpp>
#include <TritiumEngine/Rendering/TextRendering/Systems/TextRenderSystem.hpp>
#include <TritiumEngine/Utilities/Random.hpp>
#include <TritiumEngine/Utilities/Scripts/CameraStatsUI.hpp>
#include <TritiumEngine/Utilities/Scripts/FpsStatsUI.hpp>

#include <glm/gtc/constants.hpp>

using namespace RenderingBenchmark::Components;
using namespace RenderingBenchmark::Systems;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::Utilities;

using Projection = Camera::Projection;

namespace
{
  constexpr static float SCREEN_UNITS        = 100.f;
  constexpr static BlendOptions TEXT_BLEND   = {true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
  constexpr static glm::vec3 CAMERA_POSITION = {0.f, 0.f, 200.f};
  constexpr static float CUBE_SIZE           = 100.f;
} // namespace

namespace RenderingBenchmark::Scenes
{
  CubeScene::CubeScene(const std::string &name, Application &app)
      : Scene(name, app), m_cameraController(m_app.window), m_callbacks(), m_gradient() {
    // Setup color gradient
    m_gradient.addColorPoint({COLOR_RED, 0.f});
    m_gradient.addColorPoint({COLOR_YELLOW, 0.2f});
    m_gradient.addColorPoint({COLOR_GREEN, 0.4f});
    m_gradient.addColorPoint({COLOR_CYAN, 0.6f});
    m_gradient.addColorPoint({COLOR_BLUE, 0.8f});
    m_gradient.addColorPoint({COLOR_MAGENTA, 1.f});

    // Setup camera controller mappings
    m_cameraController.mapKey(Key::W, CameraAction::MOVE_FORWARD);
    m_cameraController.mapKey(Key::A, CameraAction::MOVE_LEFT);
    m_cameraController.mapKey(Key::S, CameraAction::MOVE_BACKWARD);
    m_cameraController.mapKey(Key::D, CameraAction::MOVE_RIGHT);
    m_cameraController.mapKey(Key::Q, CameraAction::MOVE_UP);
    m_cameraController.mapKey(Key::E, CameraAction::MOVE_DOWN);
    m_cameraController.mapKey(Key::LEFT, CameraAction::TURN_LEFT);
    m_cameraController.mapKey(Key::RIGHT, CameraAction::TURN_RIGHT);
    m_cameraController.mapKey(Key::UP, CameraAction::TURN_UP);
    m_cameraController.mapKey(Key::DOWN, CameraAction::TURN_DOWN);
    m_cameraController.mapKey(Key::Z, CameraAction::ZOOM_IN);
    m_cameraController.mapKey(Key::X, CameraAction::ZOOM_OUT);
    m_cameraController.mapKey(Key::C, CameraAction::CENTER);
  }

  void CubeScene::init() {
    // Set OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup systems
    addSystem<CubeRenderSystem<MainCameraTag::value>>();
    addSystem<TextRenderSystem<UiCameraTag::value>>(TEXT_BLEND);

    // Setup scene camera
    auto &registry = m_app.registry;
    auto &window   = m_app.window;

    auto sceneCamera = registry.create();
    float aspect     = window.getAspect();
    auto &sceneCameraComponent =
        m_app.registry.emplace<Camera>(sceneCamera, Projection::PERSPECTIVE, aspect, 1.f, 0.1f,
                                       500.0f, Transform{CAMERA_POSITION});
    m_app.registry.emplace<MainCameraTag>(sceneCamera);
    m_cameraController.init(sceneCameraComponent);

    // Setup UI overlay camera
    auto uiCamera = m_app.registry.create();
    registry.emplace<Camera>(uiCamera, Projection::ORTHOGRAPHIC, SCREEN_UNITS * aspect,
                             SCREEN_UNITS, 0.1f, 100.0f);
    registry.emplace<UiCameraTag>(uiCamera);

    // Setup UI
    auto fpsStatsUI = registry.create();
    auto &fpsScript =
        registry.emplace<NativeScript>(fpsStatsUI, std::make_unique<FpsStatsUI>(m_app));
    fpsScript.getInstance().setEnabled(false);

    auto camStatsUI    = registry.create();
    auto &cameraScript = registry.emplace<NativeScript>(
        camStatsUI,
        std::make_unique<CameraStatsUI>(m_app, sceneCameraComponent, m_cameraController));
    cameraScript.getInstance().setEnabled(false);

    // Setup controls
    m_callbacks[0] = window.addKeyCallback(Key::F, KeyState::RELEASED, [&registry, fpsStatsUI]() {
      registry.get<NativeScript>(fpsStatsUI).getInstance().toggleEnabled();
    });
    m_callbacks[1] = window.addKeyCallback(Key::M, KeyState::RELEASED, [&registry, camStatsUI]() {
      registry.get<NativeScript>(camStatsUI).getInstance().toggleEnabled();
    });

    generateParticles(2500000);
  }

  void CubeScene::dispose() {
    glDisable(GL_DEPTH_TEST);
    m_cameraController.dispose();
    m_app.window.removeCallbacks(m_callbacks);
  }

  void CubeScene::generateParticles(int nParticles) {
    auto &registry      = m_app.registry;
    auto &shaderManager = m_app.shaderManager;

    auto entity      = registry.create();
    auto &renderable = registry.emplace<InstancedRenderable>(
        entity, GL_POINTS, Primitives::createPoint(0.f, 0.f, 0.f), nParticles);
    registry.emplace<Shader>(entity, shaderManager.get("instanced"));

    // Set instance data
    for (int i = 0; i < nParticles; ++i) {
      const auto &pos = RandomUtils::CubePosition(CUBE_SIZE);
      float dist      = glm::length(pos);
      float maxDist   = glm::length(glm::vec3(CUBE_SIZE * 0.5f));
      Color color     = m_gradient.getColor(dist / maxDist);

      renderable.setInstanceData(i, {Transform{pos}.getModelMatrix(), color.value});
    }
    renderable.updateInstanceDataBuffer();
  }
} // namespace RenderingBenchmark::Scenes