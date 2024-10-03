#pragma once

#include <iostream>

#include "gameobject.h"

namespace game {

namespace rl = raylib;

/**
 * @class Player
 * @brief The player character. Has special treatment in the game loop.
 *
 */
class Player : public Moveable, public Textured {
public:
    Player(const rl::Vector2& position, rl::Texture&& texture,
           const rl::Vector2& velocity);

    void Draw() override;
    void Draw(float scale);
    void Draw(float scale, rl::Vector2 position);
    void Update() override;

    void Move(Direction direction) override;
    void Move(Direction direction, float amount);

    friend auto operator<<(std::ostream& os,
                           const Player& player) -> std::ostream&;

private:
    unsigned    animation_fps_ = 6;
    unsigned    frame_counter_ = 0;
    rl::Vector2 velocity_;
};

}  // namespace game
