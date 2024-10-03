#include "game.h"

#include <string>

#include "colors.h"
#include "player.h"
#include "raylib.h"

namespace game {

Game::Game(unsigned int screen_width, unsigned int screen_height,
           const std::string& title, unsigned int target_fps) noexcept
    : window_{rl::Window(screen_width, screen_height, title,
                         FLAG_WINDOW_RESIZABLE)},
      screen_width_(screen_width),
      screen_height_(screen_height),
      scale_{static_cast<float>(screen_width) / DEFAULT_SCREEN_WIDTH} {
    window_.SetTargetFPS(target_fps);
}

void Game::Run() {
    std::string player_texture_path =
        (resource_path_ / "sprites" / "jenny-walking.png").generic_string();

    rl::Texture player_texture = rl::Texture(player_texture_path);

    Player player(rl::Vector2(1 * static_cast<float>(screen_width_) / 5,
                              3 * static_cast<float>(screen_height_) / 5),
                  std::move(player_texture), rl::Vector2{10, 10});

    player_ = std::make_unique<Player>(std::move(player));

    std::string test_background_texture_path =
        (resource_path_ / "sprites" / "test-background.png").generic_string();

    rl::Texture background_texture = rl::Texture(test_background_texture_path);

    camera_.SetOffset({1 * static_cast<float>(screen_width_) / 5 + 64,
                       3 * static_cast<float>(screen_height_) / 5});
    camera_.SetZoom(1.0f);

    while (!window_.ShouldClose()) {
        if (window_.IsResized()) {
            HandleResize();
        }

        HandleKeyboardEvents();

        camera_.SetTarget(Vector2{player_->GetPositionX() + 64,
                                  3 * static_cast<float>(screen_height_) / 5});

        float height = static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);
        float width = static_cast<float>(height * 16.0F / 9.0F);

        float ratio = static_cast<float>(window_.GetWidth()) /
                      static_cast<float>(window_.GetHeight());

        if (ratio > 16.0F / 9.0F) {
            float width =
                static_cast<float>(window_.GetHeight()) * (16.0F / 9.0F);

            float height = static_cast<float>(width * 9.0F / 16.0F);

            bg_dest_rec_.x = (window_.GetWidth() - width) / 2;
            bg_dest_rec_.y = 0.0F;
            bg_dest_rec_.width = width;
            bg_dest_rec_.height = height;

        } else {
            static bool should_print = true;

            float height =
                static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);

            float width = static_cast<float>(height * 16.0F / 9.0F);

            bg_dest_rec_.x = 0.0F;
            bg_dest_rec_.y = (window_.GetHeight() - height) / 2;
            bg_dest_rec_.height = height;
            bg_dest_rec_.width = width;
        }

        window_.BeginDrawing();
        camera_.Begin();

        window_.ClearBackground(game::SPACE);

        background_texture.Draw(bg_source_rec_, bg_dest_rec_);

        std::string msg =
            "Press F to toggle camera mode. Current mode: " +
            (camera_.Mode() == Camera::Mode::kFixed ? std::string("Fixed")
                                                    : std::string("Dynamic"));
        ;
        rl::DrawText(msg, 10, 10, 20, WHITE);

        player_->Draw(scale_);

        bg_dest_rec_.DrawLines(rl::Color::RayWhite(), 2.0F);

        window_.EndDrawing();
    }
}

void Game::HandleResize() {
    screen_width_ = window_.GetWidth();
    screen_height_ = window_.GetHeight();
    scale_ = static_cast<float>(screen_width_) / DEFAULT_SCREEN_WIDTH;

    player_->SetPosition(
        rl::Vector2(1 * static_cast<float>(screen_width_) / 5,
                    3 * static_cast<float>(screen_height_) / 5));

    camera_.SetOffset({1 * static_cast<float>(screen_width_) / 5 + 64,
                       3 * static_cast<float>(screen_height_) / 5});
}

rl::Texture LoadTexture(const fs::path& texture_path) {
    return rl::Texture(texture_path.generic_string());
}

void Game::CorrectPlayerPosition() {
    if (camera_.Mode() == Camera::Mode::kDynamic) {
        if (bg_source_rec_.x + player_->GetPositionX() >
            bg_source_rec_.x + static_cast<float>(screen_width_) / 5) {
            float amount = std::min(
                10.0F, std::abs(player_->GetPositionX() - bg_source_rec_.x +
                                static_cast<float>(screen_width_) / 5));
            player_->Move(Player::Direction::kLeft, amount);
            camera_.SubtractOffsetX(amount);
        }
        if (bg_source_rec_.x + player_->GetPositionX() <
            bg_source_rec_.x + static_cast<float>(screen_width_) / 5) {
            float amount = std::min(
                10.0F, std::abs(bg_source_rec_.x + player_->GetPositionX()));
            player_->Move(Player::Direction::kRight, amount);
            camera_.AddOffsetX(amount);
        }
    }
}

}  // namespace game
