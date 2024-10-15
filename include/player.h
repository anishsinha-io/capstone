#pragma once

#include <iostream>

#include "background.h"
#include "camera.h"
#include "gameobject.h"
#include "platforms.h"

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
    void Update() override;
    void Update(int event);
    void Update(int event, Camera& camera);
    void Update(int event, Camera& camera, std::shared_ptr<Background> bg);

    void Move(Direction direction) override;
    void Move(Direction direction, float amount);

    void CheckCollision(
        std::shared_ptr<std::variant<Player, UntexturedPlatform>> platform);

    friend auto operator<<(std::ostream& os,
                           const Player& player) -> std::ostream&;

private:
    bool CheckLeftCollision(
        std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor);

    bool CheckRightCollision(
        std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor);

    bool CheckTopCollision(
        std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor);

    bool CheckBottomCollision(
        std::shared_ptr<std::variant<Player, UntexturedPlatform>> actor);

    unsigned animation_fps_ = 6;
    unsigned frame_counter_ = 0;
    std::array<std::optional<
                   std::shared_ptr<std::variant<Player, UntexturedPlatform>>>,
               4>
        collision_objects_ = {std::nullopt, std::nullopt, std::nullopt,
                              std::nullopt};  // left, right, top, bottom
};

}  // namespace game
