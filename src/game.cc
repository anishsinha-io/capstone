#include "game.h"

#include <iostream>

#include "Keyboard.hpp"
#include "player.h"
#include "raylib.h"

namespace game {

Game::Game(unsigned int screen_width, unsigned int screen_height,
           const std::string& title, unsigned int target_fps) noexcept
    : window_{rl::Window(screen_width, screen_height, title)} {
    window_.SetTargetFPS(target_fps);
}

void Game::Run() {
    std::string player_texture_path =
        resource_path_ / "sprites" / "jenny-walking.png";

    rl::Texture player_texture = rl::Texture(player_texture_path);
    Player      player(rl::Vector2(327.0F, 327.0F), std::move(player_texture),
                       rl::Vector2{10, 10});

    std::cout << player << std::endl;

    while (!window_.ShouldClose()) {
        window_.BeginDrawing();

        window_.ClearBackground(rl::Color::RayWhite());

        rl::DrawText("Congrats! You created your first window!", 190, 200, 20,
                     rl::Color::Black());

        player.Draw();

        window_.EndDrawing();
    }
}

rl::Texture LoadTexture(const fs::path& texture_path) {
    return rl::Texture(texture_path.string());
}

/**
 * @brief Handle keyboard events, for example arrow keys, space bar, etc.
 */
void Game::HandleKeyboardEvents() {
    int key = rl::Keyboard::GetKeyPressed();
    switch (key) {
        case KEY_RIGHT:
            break;
        case KEY_LEFT:
            break;
        case KEY_UP:
            break;
        case KEY_DOWN:
            break;
        case KEY_P:
            break;
        case KEY_R:
            break;
        default:
            break;
    };
}

}  // namespace game
