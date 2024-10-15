#pragma once

#include <filesystem>
#include <raylib-cpp.hpp>

#include "actor.h"
#include "background.h"
#include "camera.h"

namespace game {

#define DEFAULT_SCREEN_WIDTH 1200
#define DEFAULT_SCREEN_HEIGHT 675

namespace fs = std::filesystem;
namespace rl = raylib;

/**
 * @class Game
 * @brief The main game class. Contains the game loop and all game objects.
 *
 */
class Game {
public:
    enum class CameraMode { kDynamic, kFixed };

    Game(unsigned int screen_width, unsigned int screen_height,
         const std::string& title, unsigned int target_fps) noexcept;
    ~Game() {}

    void Run();

private:
    void        HandleResize();
    void        HandleKeyboardEvents();
    inline void SetBgSourceRec(const rl::Rectangle& bg_source_rec) {
        background_->source_rec_ = bg_source_rec;
    }
    inline void SetBgDestRec(const rl::Rectangle& bg_dest_rec) {
        background_->dest_rec_ = bg_dest_rec;
    }
    // inline rl::Rectangle GetBgSourceRec() const { return bg_source_rec_; }
    // inline rl::Rectangle GetBgDestRec() const { return bg_dest_rec_; }

    inline std::shared_ptr<Background> GetBackground() { return background_; }

    void CorrectPlayerPosition();
    void InitGame();

    static inline fs::path resource_path_ =
        fs::current_path().append("resources");
    unsigned int                        level_ = 1;
    unsigned int                        target_fps_ = 60;
    unsigned int                        screen_width_;
    unsigned int                        screen_height_;
    std::string                         title_ = "Roguelike";
    rl::Window                          window_;
    float                               scale_ = 1.0F;
    Camera                              camera_;
    std::vector<std::shared_ptr<Actor>> actors_;
    std::shared_ptr<Background>         background_;
};

}  // namespace game
