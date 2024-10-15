#include "game.h"

#include <string>

#include "colors.h"
#include "player.h"
#include "raylib.h"

namespace game {

void Game::InitGame() {
    std::string player_texture_path =
        (resource_path_ / "sprites" / "jenny-walking.png").generic_string();

    rl::Texture player_texture = rl::Texture(player_texture_path);

    Player player(rl::Vector2(1 * static_cast<float>(screen_width_) / 5,
                              3 * static_cast<float>(screen_height_) / 5),
                  std::move(player_texture), rl::Vector2{10, 10});

    rl::Rectangle bg_source_rec = {0.0F, 0.0F, 1600.0f, 900.0f};
    rl::Rectangle bg_dest_rec = {0.0F, 0.0F,
                                 static_cast<float>(window_.GetWidth()),
                                 static_cast<float>(window_.GetHeight())};

    std::string test_background_texture_path =
        (resource_path_ / "sprites" / "test-background.png").generic_string();
    rl::Texture background_texture = rl::Texture(test_background_texture_path);

    background_ = std::make_shared<Background>(bg_source_rec, bg_dest_rec,
                                               std::move(background_texture));

    actors_.push_back(std::make_shared<Actor>(std::move(player)));

    camera_.SetOffset({1 * static_cast<float>(screen_width_) / 5 + 64,
                       3 * static_cast<float>(screen_height_) / 5});
    camera_.SetZoom(1.0f);

    UntexturedPlatform wall1(rl::Vector2(20.0F, 300.0F),
                             rl::Vector2(10.0F, 700.0F), rl::Color::Red(),
                             scale_);

    UntexturedPlatform wall2(rl::Vector2(990.0F, 300.0F),
                             rl::Vector2(10.0F, 700.0F), rl::Color::Red(),
                             scale_);

    UntexturedPlatform floor(rl::Vector2(20.0F, 600.0F),
                             rl::Vector2(1000.0F, 10.0F), rl::Color::Green(),
                             scale_);

    UntexturedPlatform ceiling(rl::Vector2(200.0F, 300.0F),
                               rl::Vector2(200.0F, 10.0F), rl::Color::Green(),
                               scale_);

    actors_.push_back(std::move(std::make_shared<Actor>(wall1)));
    actors_.push_back(std::move(std::make_shared<Actor>(wall2)));
    actors_.push_back(std::move(std::make_shared<Actor>(floor)));
    actors_.push_back(std::move(std::make_shared<Actor>(ceiling)));
}

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
    InitGame();

    while (!window_.ShouldClose()) {
        if (window_.IsResized()) {
            HandleResize();
        }

        HandleKeyboardEvents();

        auto player = std::get_if<Player>(actors_[0].get());

        camera_.SetTarget(Vector2{player->GetPositionX() + 64,
                                  3 * static_cast<float>(screen_height_) / 5});

        float height = static_cast<float>(window_.GetWidth()) * (9.0F / 16.0F);
        float width = static_cast<float>(height * 16.0F / 9.0F);

        background_->MakeAspectRatio(width, height);

        window_.BeginDrawing();
        camera_.Begin();

        window_.ClearBackground(game::SPACE);

        background_->Draw();

        std::string msg =
            "Press F to toggle camera mode. Current mode: " +
            (camera_.Mode() == Camera::Mode::kFixed ? std::string("Fixed")
                                                    : std::string("Dynamic"));
        ;
        rl::DrawText(msg, 10, 10, 20, WHITE);

        for (auto& actor : actors_) {
            std::visit([&](auto&& arg) { arg.Draw(); }, *actor);
        }

        player->Draw();

        // bg_dest_rec_.DrawLines(rl::Color::RayWhite(), 2.0F);

        window_.EndDrawing();
    }
}

void Game::HandleResize() {
    screen_width_ = window_.GetWidth();
    screen_height_ = window_.GetHeight();
    scale_ = static_cast<float>(screen_width_) / DEFAULT_SCREEN_WIDTH;

    auto player = std::get_if<Player>(actors_[0].get());

    player->SetPosition(
        rl::Vector2(1 * static_cast<float>(screen_width_) / 5,
                    3 * static_cast<float>(screen_height_) / 5));

    camera_.SetOffset({1 * static_cast<float>(screen_width_) / 5 + 64,
                       3 * static_cast<float>(screen_height_) / 5});
}

void Game::HandleKeyboardEvents() {
    auto player = std::get_if<Player>(actors_[0].get());

    for (int i = 1; i < actors_.size(); i++) {
        for (int j = 1; j < actors_.size(); j++) {
            player->CheckCollision(actors_[i]);

            if (i != j) {
            }
        }
    }

    if (IsKeyDown(KEY_A)) {
        player->Update(KEY_A, camera_, background_);
    }
    if (IsKeyDown(KEY_D)) {
        player->Update(KEY_D, camera_, background_);
    }
    if (IsKeyDown(KEY_W)) {
        player->Update(KEY_W, camera_, background_);
    }
    if (IsKeyDown(KEY_S)) {
        player->Update(KEY_S, camera_, background_);
    }

    if (IsKeyDown(KEY_UP)) {
        if (camera_.Zoom() > 2.0f) {
            return;
        }
        camera_.AddZoom(0.1F);
    } else if (IsKeyDown(KEY_DOWN)) {
        if (camera_.Zoom() < 0.4f) {
            return;
        }
        camera_.SubtractZoom(0.1F);
    }
    if (IsKeyPressed(KEY_F)) {
        camera_.ToggleMode();
    }
}

}  // namespace game
