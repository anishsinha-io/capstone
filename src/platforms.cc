#include "platforms.h"

namespace game {

UntexturedPlatform::UntexturedPlatform(const rl::Vector2& position,
                                       const rl::Vector2& dimensions,
                                       const rl::Color color, float scale)
    : GameObject(position, scale), dimensions_(dimensions), color_(color) {}

void UntexturedPlatform::Draw() {
    rl::Rectangle rec = {position_.x, position_.y, dimensions_.x,
                         dimensions_.y};

    rec.Draw(color_);
}

void UntexturedPlatform::Update() {}

}  // namespace game
