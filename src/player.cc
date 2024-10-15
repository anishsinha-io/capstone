#include "player.h"

namespace game {

Player::Player(const rl::Vector2& position, rl::Texture&& texture,
               const rl::Vector2& velocity)
    : Textured(std::move(texture)), GameObject(position) {}

void Player::Move(Direction direction, float amount) {
    switch (direction) {
        case Direction::kUp:
            position_.y -= amount;
            break;
        case Direction::kDown:
            position_.y += amount;
            break;
        case Direction::kLeft:
            position_.x -= amount;
            break;
        case Direction::kRight:
            position_.x += amount;
            break;
    }
}

void Player::Move(Direction direction) { Move(direction, 10.0F); }

auto operator<<(std::ostream& os, const Player& player) -> std::ostream& {
    os << "Player position: " << player.position_.x << ", "
       << player.position_.y << std::endl;
    return os;
}

void Player::Draw() {
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

    float width = scale_ * static_cast<float>(texture_.GetWidth()) / 6;
    float height = scale_ * static_cast<float>(texture_.GetHeight());

    rl::Rectangle dest_rec = {position_.x, position_.y, width, height};

    texture_.Draw(source_rec, dest_rec, {0, 0});
}

bool Player::CheckLeftCollision(
    std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor) {
    auto x = std::visit(
        [&](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, UntexturedPlatform>) {
                bool left_boundary_is_less_than_actor_right_boundary =
                    position_.x < arg.GetPosition().x + arg.GetDimensions().x;

                bool right_boundary_is_greater_than_actor_right_boundary =
                    position_.x + 128 >
                    arg.GetPosition().x + arg.GetDimensions().x;

                bool top_boundary_is_less_than_actor_bottom_boundary =
                    position_.y < arg.GetPosition().y + arg.GetDimensions().y;

                bool bottom_boundary_is_greater_than_actor_top_boundary =
                    position_.y + 128 > arg.GetPosition().y;

                bool top_boundary_is_greater_than_actor_top_boundary =
                    position_.y > arg.GetPosition().y;

                bool bottom_boundary_is_less_than_actor_bottom_boundary =
                    position_.y + 128 <
                    arg.GetPosition().y + arg.GetDimensions().y;

                return left_boundary_is_less_than_actor_right_boundary &&
                       right_boundary_is_greater_than_actor_right_boundary &&
                       (top_boundary_is_less_than_actor_bottom_boundary &&
                            top_boundary_is_greater_than_actor_top_boundary ||
                        bottom_boundary_is_greater_than_actor_top_boundary &&
                            bottom_boundary_is_less_than_actor_bottom_boundary);
            }
            return false;
        },
        *actor);
    return x;
}

bool Player::CheckRightCollision(
    std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor) {
    bool x = std::visit(
        [&](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, UntexturedPlatform>) {
                bool right_boundary_is_greater_than_actor_left_boundary =
                    position_.x + 128 > arg.GetPosition().x;

                bool left_boundary_is_less_than_actor_left_boundary =
                    position_.x < arg.GetPosition().x;

                bool top_boundary_is_less_than_actor_bottom_boundary =
                    position_.y < arg.GetPosition().y + arg.GetDimensions().y;

                bool bottom_boundary_is_greater_than_actor_top_boundary =
                    position_.y + 128 > arg.GetPosition().y;

                bool top_boundary_is_greater_than_actor_top_boundary =
                    position_.y > arg.GetPosition().y;

                bool bottom_boundary_is_less_than_actor_bottom_boundary =
                    position_.y + 128 <
                    arg.GetPosition().y + arg.GetDimensions().y;

                return right_boundary_is_greater_than_actor_left_boundary &&
                       ((top_boundary_is_less_than_actor_bottom_boundary &&
                         top_boundary_is_greater_than_actor_top_boundary) ||
                        (bottom_boundary_is_greater_than_actor_top_boundary &&
                         bottom_boundary_is_less_than_actor_bottom_boundary)) &&
                       left_boundary_is_less_than_actor_left_boundary;

            } else {
                return false;
            }
        },
        *actor);

    return x;
}

bool Player::CheckTopCollision(
    std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor) {
    bool x = std::visit(
        [&](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, UntexturedPlatform>) {
                bool top_boundary_is_less_than_actor_bottom_boundary =
                    position_.y < arg.GetPosition().y + arg.GetDimensions().y;

                bool top_boundary_is_greater_than_actor_top_boundary =
                    position_.y > arg.GetPosition().y;

                bool left_boundary_is_less_than_actor_right_boundary =
                    position_.x < arg.GetPosition().x + arg.GetDimensions().x;

                bool left_boundary_is_greater_than_actor_left_boundary =
                    position_.x > arg.GetPosition().x;

                bool right_boundary_is_greater_than_actor_left_boundary =
                    position_.x + 128 > arg.GetPosition().x;

                bool right_boundary_is_less_than_actor_right_boundary =
                    position_.x + 128 <
                    arg.GetPosition().x + arg.GetDimensions().x;

                return top_boundary_is_less_than_actor_bottom_boundary &&
                       top_boundary_is_greater_than_actor_top_boundary &&
                       ((left_boundary_is_less_than_actor_right_boundary &&
                         left_boundary_is_greater_than_actor_left_boundary) ||
                        (right_boundary_is_greater_than_actor_left_boundary &&
                         right_boundary_is_less_than_actor_right_boundary));

            } else {
                return false;
            }
        },
        *actor);

    return x;
}

bool Player::CheckBottomCollision(
    std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor) {
    bool x = std::visit(
        [&](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, UntexturedPlatform>) {
                bool bottom_boundary_is_greater_than_actor_top_boundary =
                    position_.y + 128 > arg.GetPosition().y;

                bool bottom_boundary_is_less_than_actor_bottom_boundary =
                    position_.y + 128 <
                    arg.GetPosition().y + arg.GetDimensions().y;

                bool left_boundary_is_less_than_actor_right_boundary =
                    position_.x < arg.GetPosition().x + arg.GetDimensions().x;

                bool left_boundary_is_greater_than_actor_left_boundary =
                    position_.x > arg.GetPosition().x;

                bool right_boundary_is_greater_than_actor_left_boundary =
                    position_.x + 128 > arg.GetPosition().x;

                bool right_boundary_is_less_than_actor_right_boundary =
                    position_.x + 128 <
                    arg.GetPosition().x + arg.GetDimensions().x;

                return bottom_boundary_is_greater_than_actor_top_boundary &&
                       bottom_boundary_is_less_than_actor_bottom_boundary &&
                       ((left_boundary_is_less_than_actor_right_boundary &&
                         left_boundary_is_greater_than_actor_left_boundary) ||
                        (right_boundary_is_greater_than_actor_left_boundary &&
                         right_boundary_is_less_than_actor_right_boundary));

            } else {
                return false;
            }
        },
        *actor);

    return x;
}

void Player::CheckCollision(
    std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor) {
    if (CheckLeftCollision(actor)) {
        collision_objects_[0] = actor;
        collision_objects_[1] = std::nullopt;
    }
    if (CheckRightCollision(actor)) {
        collision_objects_[1] = actor;
        collision_objects_[0] = std::nullopt;
    }
    if (CheckTopCollision(actor)) {
        collision_objects_[2] = actor;
        collision_objects_[3] = std::nullopt;
    }
    if (CheckBottomCollision(actor)) {
        collision_objects_[3] = actor;
        collision_objects_[2] = std::nullopt;
    }
}

void Player::Update() {}
void Player::Update(int event) {}
void Player::Update(int event, Camera& camera) {}

void Player::Update(int event, Camera& camera, std::shared_ptr<Background> bg) {
    if (event == KEY_A && !collision_objects_[0].has_value()) {
        collision_objects_[1] = std::nullopt;
        float xspeed = std::abs(velocity_.x);
        if (velocity_.x > 0.0F) {
            velocity_.x = 0.0F;
        }

        if (xspeed < 10.0F) {
            velocity_.x -= 0.1F;
        }

        xspeed = std::abs(velocity_.x);

        bg->source_rec_.x -= xspeed;
        bg->dest_rec_.x -= xspeed;
        if (camera.Mode() == Camera::Mode::kFixed &&
            position_.x > bg->dest_rec_.x) {
            camera.SubtractOffsetX(xspeed);
            position_.x -= xspeed;
        }
    }
    if (event == KEY_D && !collision_objects_[1].has_value()) {
        collision_objects_[0] = std::nullopt;
        float xspeed = std::abs(velocity_.x);
        if (velocity_.x < 0.0F) {
            velocity_.x = 0.0F;
        }
        if (xspeed < 10.0F) {
            velocity_.x += 0.1F;
        }

        xspeed = std::abs(velocity_.x);

        bg->source_rec_.x += xspeed;
        bg->dest_rec_.x += xspeed;

        if (camera.Mode() == Camera::Mode::kFixed &&
            position_.x < bg->dest_rec_.x + bg->dest_rec_.width - 128) {
            camera.AddOffsetX(xspeed);
            position_.x += xspeed;
        }
    }

    if (event == KEY_W && !collision_objects_[2].has_value()) {
        collision_objects_[3] = std::nullopt;
        float yspeed = std::abs(velocity_.y);
        if (velocity_.y > 0.0F) {
            velocity_.y = 0.0F;
        }

        if (yspeed < 10.0F) {
            velocity_.y -= 0.1F;
        }

        yspeed = std::abs(velocity_.y);

        position_.y -= yspeed;
    }
    if (event == KEY_S && !collision_objects_[3].has_value()) {
        collision_objects_[2] = std::nullopt;
        float yspeed = std::abs(velocity_.y);
        if (velocity_.y < 0.0F) {
            velocity_.y = 0.0F;
        }

        if (yspeed < 10.0F) {
            velocity_.y += 0.1F;
        }

        yspeed = std::abs(velocity_.y);

        position_.y += yspeed;
    }
}

}  // namespace game
