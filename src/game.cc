#include "game.h"

namespace game {

Game::Game(unsigned int screen_width, unsigned int screen_height,
           const std::string& title, unsigned int target_fps) noexcept
    : window_{raylib::Window(screen_width, screen_height, title)} {
    window_.SetTargetFPS(target_fps);
}

void Game::Run() {
    while (!window_.ShouldClose()) {
        window_.BeginDrawing();

        window_.ClearBackground(raylib::Color::RayWhite());

        raylib::DrawText("Congrats! You created your first window!", 190, 200,
                         20, raylib::Color::Black());

        window_.EndDrawing();
    }
}

}  // namespace game
