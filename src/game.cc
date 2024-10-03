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

    // camera_.target =
    //     rl::Vector2{player_->GetPositionX() + 64, player_->GetPositionY() +
    //     64};

    camera_.offset = {1 * static_cast<float>(screen_width_) / 5 + 64,
                      3 * static_cast<float>(screen_height_) / 5};
    camera_.zoom = 1.0f;

    while (!window_.ShouldClose()) {
        if (window_.IsResized()) {
            HandleResize();
            // player_->SetPosition(static_cast<float>(screen_width_) / 2 - 64,
            //                      static_cast<float>(screen_height_) / 2 -
            //                      64);
        }

        HandleKeyboardEvents();

        camera_.target = Vector2{player_->GetPositionX() + 64,
                                 3 * static_cast<float>(screen_height_) / 5};

        float height = static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);
        float width = static_cast<float>(height * 16.0F / 9.0F);

        float ratio = static_cast<float>(window_.GetWidth()) /
                      static_cast<float>(window_.GetHeight());

        if (ratio > 16.0F / 9.0F) {
            float width =
                static_cast<float>(window_.GetHeight()) * (16.0F / 9.0F);

            bg_dest_rec_.x = (window_.GetWidth() - width) / 2;
            bg_dest_rec_.y = 0.0F;
            bg_dest_rec_.width = width;

            // player_->SetPositionX(bg_dest_rec_.x + bg_dest_rec_.width / 6);
            // player_->SetPositionY(bg_dest_rec_.y + bg_dest_rec_.height * 3 /
            // 5);

        } else {
            float height =
                static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);

            bg_dest_rec_.x = 0.0F;
            bg_dest_rec_.y = (window_.GetHeight() - height) / 2;
            bg_dest_rec_.height = height;

            // player_->SetPositionX(bg_dest_rec_.x + bg_dest_rec_.width / 6);
            // player_->SetPositionY(bg_dest_rec_.y +
            //                       bg_dest_rec_.height * 7 / 10);
        }

        window_.BeginDrawing();
        BeginMode2D(camera_);

        window_.ClearBackground(game::SPACE);

        background_texture.Draw(bg_source_rec_, bg_dest_rec_);

        std::string msg =
            "Press F to toggle camera mode. Current mode: " +
            (camera_mode_ == CameraMode::kFixed ? std::string("Fixed")
                                                : std::string("Dynamic"));
        ;
        rl::DrawText(msg, 10, 10, 20, WHITE);

        player_->Draw(scale_);

        bg_dest_rec_.DrawLines(rl::Color::RayWhite(), 2.0F);

        EndMode2D();

        window_.EndDrawing();
    }
}

void Game::HandleResize() {
    screen_width_ = window_.GetWidth();
    screen_height_ = window_.GetHeight();
    scale_ = static_cast<float>(screen_width_) / DEFAULT_SCREEN_WIDTH;
}

rl::Texture LoadTexture(const fs::path& texture_path) {
    return rl::Texture(texture_path.generic_string());
}

/**
 * @brief Handle keyboard events, for example arrow keys, space bar, etc.
 */
void Game::HandleKeyboardEvents() {
    if (IsKeyDown(KEY_A)) {
        bg_source_rec_.x -= 10;
        bg_dest_rec_.x -= 10;
        if (camera_mode_ == CameraMode::kFixed &&
            player_->GetPositionX() > bg_dest_rec_.x) {
            camera_.offset.x -= 10;
            player_->SetPositionX(player_->GetPositionX() - 10);
        }
    }
    if (IsKeyDown(KEY_D)) {
        bg_source_rec_.x += 10;
        bg_dest_rec_.x += 10;

        if (camera_mode_ == CameraMode::kFixed &&
            player_->GetPositionX() <
                bg_dest_rec_.x + bg_dest_rec_.width - 128) {
            camera_.offset.x += 10;
            player_->SetPositionX(player_->GetPositionX() + 10);
        }
    }
    if (IsKeyDown(KEY_W)) {
        player_->Move(Player::Direction::kUp);
    }
    if (IsKeyDown(KEY_S)) {
        player_->Move(Player::Direction::kDown);
    }
    if (IsKeyDown(KEY_UP)) {
        if (camera_.zoom > 2.0f) {
            return;
        }
        camera_.zoom += 0.1F;
    } else if (IsKeyDown(KEY_DOWN)) {
        if (camera_.zoom < 0.4f) {
            return;
        }
        camera_.zoom -= 0.1F;
    }
    if (IsKeyPressed(KEY_F)) {
        camera_mode_ = camera_mode_ == CameraMode::kFixed ? CameraMode::kDynamic
                                                          : CameraMode::kFixed;
    }
    if (camera_mode_ == CameraMode::kDynamic) {
        if (bg_source_rec_.x + player_->GetPositionX() >
            bg_source_rec_.x + static_cast<float>(screen_width_) / 5) {
            float amount = std::min(
                10.0F, std::abs(player_->GetPositionX() - bg_source_rec_.x +
                                static_cast<float>(screen_width_) / 5));
            player_->Move(Player::Direction::kLeft, amount);
            camera_.offset.x -= amount;
        }
        if (bg_source_rec_.x + player_->GetPositionX() <
            bg_source_rec_.x + static_cast<float>(screen_width_) / 5) {
            // camera_.offset.x = player_->GetPositionX() + 64;
            float amount = std::min(
                10.0F, std::abs(bg_source_rec_.x + player_->GetPositionX()));
            player_->Move(Player::Direction::kRight, amount);
            camera_.offset.x += amount;
        }
    }
}

}  // namespace game
