#include "player.h"

namespace game {

Player::Player(const rl::Vector2& position, rl::Texture&& texture,
               const rl::Vector2& velocity)
    : Textured(std::move(texture)), GameObject(position) {}

void Player::Move(Direction direction) {
    switch (direction) {
        case Direction::kUp:
            position_.y -= velocity_.y;
            break;
        case Direction::kDown:
            position_.y += velocity_.y;
            break;
        case Direction::kLeft:
            position_.x -= velocity_.x;
            break;
        case Direction::kRight:
            position_.x += velocity_.x;
            break;
    }
}

auto operator<<(std::ostream& os, const Player& player) -> std::ostream& {
    os << "Player position: " << player.position_.x << ", "
       << player.position_.y << std::endl;
    return os;
}

void Player::Draw() { Draw(0.0F); }

void Player::Draw(float scale) {
    static rl::Rectangle frame_rec = {
        position_.x, position_.y, static_cast<float>(texture_.GetWidth()) / 6,
        static_cast<float>(texture_.GetHeight())};

    frame_counter_ += 1;
    if (frame_counter_ >= 12) {
        frame_counter_ = 0;
        frame_rec.x += static_cast<float>(texture_.GetWidth()) / 6;

        if (frame_rec.x >=
            static_cast<float>(texture_.GetWidth()) + position_.x) {
            frame_rec.x = position_.x;
        }
    }

    rl::Rectangle rec = {position_.x, position_.y,
                         static_cast<float>(texture_.GetWidth()),
                         static_cast<float>(texture_.GetHeight())};

    float x = (static_cast<int>(frame_rec.x) + 127) & ~127;
    float y = (static_cast<int>(frame_rec.y) + 127) & ~127;

    rl::Rectangle source_rec = {x, y,
                                static_cast<float>(texture_.GetWidth()) / 6,
                                static_cast<float>(texture_.GetHeight())};

    rl::Rectangle dest_rec = {position_.x, position_.y,
                              static_cast<float>(texture_.GetWidth()) / 6,
                              static_cast<float>(texture_.GetHeight())};

    texture_.Draw(source_rec, dest_rec, {0, 0});
}

void Player::Draw(float scale, rl::Vector2 position) {
    position_ = position;
    Draw(scale);
}

}  // namespace game
