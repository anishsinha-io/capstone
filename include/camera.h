#pragma once

#include <raylib-cpp.hpp>

namespace game {

namespace rl = raylib;

class Camera {
public:
    Camera() noexcept;
    Camera(const rl::Vector2& target, const rl::Vector2& offset,
           float zoom) noexcept;
    ~Camera() { EndMode2D(); }

    inline void SetTarget(const rl::Vector2& target) {
        camera_.target = target;
    }

    inline void SetTargetX(float x) { camera_.target.x = x; }
    inline void SetTargetY(float y) { camera_.target.y = y; }

    inline void SetOffset(const rl::Vector2& offset) {
        camera_.offset = offset;
    }

    inline void SetOffsetX(float x) { camera_.offset.x = x; }
    inline void SetOffsetY(float y) { camera_.offset.y = y; }

    inline void SetZoom(float zoom) { camera_.zoom = zoom; }

    inline rl::Vector2 GetTarget() const { return camera_.target; }
    inline rl::Vector2 GetOffset() const { return camera_.offset; }
    inline float       GetZoom() const { return camera_.zoom; }

    inline void Begin() { BeginMode2D(camera_); }

private:
    rl::Camera2D camera_ = {};
};

}  // namespace game
