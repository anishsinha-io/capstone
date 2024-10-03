#pragma once

#include <raylib.h>

#include <raylib-cpp.hpp>

namespace game {

namespace rl = raylib;

/**
 * @class Camera
 * @brief C++ abstraction over Raylib's Camera2D struct. The bindings provided
 * by <raylib-cpp.hpp> are insufficient.
 *
 */
class Camera {
public:
    enum class Mode { kDynamic, kFixed };

    ~Camera() { EndMode2D(); }

    inline void SetTarget(const rl::Vector2& target) {
        camera_.target = target;
    }

    inline void SetTargetX(float x) { camera_.target.x = x; }
    inline void SetTargetY(float y) { camera_.target.y = y; }

    inline void SetOffset(const rl::Vector2& offset) {
        camera_.offset = offset;
    }

    inline void SetMode(Mode mode) { mode_ = mode; }
    inline Mode Mode() const { return mode_; }
    inline void ToggleMode() {
        mode_ = (mode_ == Mode::kDynamic) ? Mode::kFixed : Mode::kDynamic;
    }

    inline void SetOffsetX(float x) { camera_.offset.x = x; }
    inline void SetOffsetY(float y) { camera_.offset.y = y; }

    inline float OffsetX() const { return camera_.offset.x; }
    inline float OffsetY() const { return camera_.offset.y; }
    inline void  SubtractOffsetX(float x) { camera_.offset.x -= x; }
    inline void  SubtractOffsetY(float y) { camera_.offset.y -= y; }
    inline void  AddOffsetX(float x) { camera_.offset.x += x; }
    inline void  AddOffsetY(float y) { camera_.offset.y += y; }

    inline void  SetZoom(float zoom) { camera_.zoom = zoom; }
    inline void  AddZoom(float zoom) { camera_.zoom += zoom; }
    inline void  SubtractZoom(float zoom) { camera_.zoom -= zoom; }
    inline float Zoom() const { return camera_.zoom; }

    inline rl::Vector2 GetTarget() const { return camera_.target; }
    inline rl::Vector2 GetOffset() const { return camera_.offset; }

    inline void Begin() { BeginMode2D(camera_); }

private:
    Camera2D  camera_ = {0};
    enum Mode mode_ = Mode::kDynamic;
};

}  // namespace game
