#include "game.h"

namespace game {
/**
 * @brief Handle keyboard events, for example arrow keys, space bar, etc.
 */
void Game::HandleKeyboardEvents() {
    if (IsKeyDown(KEY_A)) {
        bg_source_rec_.x -= 10;
        bg_dest_rec_.x -= 10;
        if (camera_.Mode() == Camera::Mode::kFixed &&
            player_->GetPositionX() > bg_dest_rec_.x) {
            camera_.SubtractOffsetX(10.0f);
            player_->SetPositionX(player_->GetPositionX() - 10);
        }
    }
    if (IsKeyDown(KEY_D)) {
        bg_source_rec_.x += 10;
        bg_dest_rec_.x += 10;

        if (camera_.Mode() == Camera::Mode::kFixed &&
            player_->GetPositionX() <
                bg_dest_rec_.x + bg_dest_rec_.width - 128) {
            camera_.AddOffsetX(10.0f);
            player_->SetPositionX(player_->GetPositionX() + 10);
        }
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
    CorrectPlayerPosition();
}

}  // namespace game
