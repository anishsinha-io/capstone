#pragma once

#include <raylib-cpp.hpp>

namespace game {

namespace rl = raylib;
class Background {
    friend class Game;
    friend class Player;

public:
    Background(const rl::Rectangle& source_rec, const rl::Rectangle& dest_rec,
               rl::Texture&& texture)
        : source_rec_{source_rec},
          dest_rec_{dest_rec},
          texture_{std::move(texture)} {}

    void Draw() {
        texture_.Draw(source_rec_, dest_rec_);
        dest_rec_.DrawLines(rl::Color::RayWhite(), 2.0F);
    }

    void SetSourceRec(const rl::Rectangle& source_rec) {
        source_rec_ = source_rec;
    }

    void SetDestRec(const rl::Rectangle& dest_rec) { dest_rec_ = dest_rec; }

    auto GetSourceRec() const -> const rl::Rectangle& { return source_rec_; }

    auto GetDestRec() const -> const rl::Rectangle& { return dest_rec_; }

    void MakeAspectRatio(float window_width, float window_height) {
        float ratio = static_cast<float>(window_width) /
                      static_cast<float>(window_height);

        if (ratio > 16.0F / 9.0F) {
            float width = static_cast<float>(window_height * (16.0F / 9.0F));

            float height = static_cast<float>(width * 9.0F / 16.0F);

            dest_rec_.x = (window_width - width) / 2;
            dest_rec_.y = 0.0F;
            dest_rec_.width = width;
            dest_rec_.height = height;

        } else {
            float height = static_cast<float>(window_width * (9.0F / 16.0F));

            float width = static_cast<float>(height * 16.0F / 9.0F);

            dest_rec_.x = 0.0F;
            dest_rec_.y = (window_height - height) / 2;
            dest_rec_.height = height;
            dest_rec_.width = width;
        }
    }

private:
    rl::Texture   texture_;
    rl::Rectangle source_rec_;
    rl::Rectangle dest_rec_;
};

}  // namespace game
