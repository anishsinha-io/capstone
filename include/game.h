#pragma once

#include <filesystem>
#include <raylib-cpp.hpp>
#include <vector>

#include "gameobject.h"
#include "player.h"

namespace game {

#define DEFAULT_SCREEN_WIDTH 1200
#define DEFAULT_SCREEN_HEIGHT 675

namespace fs = std::filesystem;
namespace rl = raylib;

class Game {
public:
    enum class CameraMode { kDynamic, kFixed };

    Game(unsigned int screen_width, unsigned int screen_height,
         const std::string& title, unsigned int target_fps) noexcept;
    ~Game() {}

    void        Run();
    void        HandleResize();
    void        HandleKeyboardEvents();
    inline void SetBgSourceRec(const rl::Rectangle& bg_source_rec) {
        bg_source_rec_ = bg_source_rec;
    }

    inline void SetBgDestRec(const rl::Rectangle& bg_dest_rec) {
        bg_dest_rec_ = bg_dest_rec;
    }

    inline rl::Rectangle GetBgSourceRec() const { return bg_source_rec_; }
    inline rl::Rectangle GetBgDestRec() const { return bg_dest_rec_; }

private:
    static inline fs::path resource_path_ =
        fs::current_path().append("resources");
    unsigned int                             level_ = 1;
    unsigned int                             target_fps_ = 60;
    unsigned int                             screen_width_;
    unsigned int                             screen_height_;
    std::string                              title_ = "Roguelike";
    rl::Window                               window_;
    float                                    scale_ = 1.0F;
    std::unique_ptr<Player>                  player_;
    std::vector<std::unique_ptr<GameObject>> game_objects_;
    rl::Camera2D                             camera_ = {};
    CameraMode    camera_mode_ = CameraMode::kDynamic;
    rl::Rectangle bg_source_rec_ = {0.0F, 0.0F, 1600.0f, 900.0f};
    rl::Rectangle bg_dest_rec_ = {0.0F, 0.0F,
                                  static_cast<float>(window_.GetWidth()),
                                  static_cast<float>(window_.GetHeight())};
};

rl::Texture LoadTexture(const fs::path& texture_path);

}  // namespace game
