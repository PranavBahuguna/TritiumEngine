#pragma once

#include <TritiumEngine/Input/InputManager.hpp>
#include <TritiumEngine/Rendering/Components/Camera.hpp>

using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;

namespace TritiumEngine::Utilities
{
  enum class CameraAction : uint8_t {
    MOVE_FORWARD = 0,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_UP,
    TURN_DOWN,
    ZOOM_IN,
    ZOOM_OUT,
    CENTER
  };

  class CameraController {
  public:
    CameraController(InputManager &inputManager);

    void init(Camera &camera);
    void dispose();
    void mapKey(Key key, CameraAction action);

    float getPitch() const { return m_pitch; }
    float getYaw() const { return m_yaw; }

  private:
    void addPitch(float pitch);
    void addYaw(float yaw);
    void addZoom(Camera &camera, float zoom);

    InputManager &m_input;
    std::unordered_map<CameraAction, Key> m_actionKey;
    CallbackId m_callbacks[15];
    float m_yaw;
    float m_pitch;
  };
} // namespace TritiumEngine::Utilities