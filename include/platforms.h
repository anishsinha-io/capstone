#pragma once

#include <raylib-cpp.hpp>

#include "gameobject.h"

namespace game {

namespace rl = raylib;

class UntexturedPlatform : public GameObject {
public:
    UntexturedPlatform(const rl::Vector2& position,
                       const rl::Vector2& dimensions, const rl::Color color,
                       float scale);

    void Draw() override;
    void Update() override;

    inline rl::Vector2 GetDimensions() const { return dimensions_; }

private:
    rl::Vector2 dimensions_;
    rl::Color   color_;
};

}  // namespace game
