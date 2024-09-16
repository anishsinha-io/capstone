#pragma once

#include <filesystem>
#include <raylib-cpp.hpp>

namespace game {

namespace fs = std::filesystem;
namespace rl = raylib;

class Game {
public:
    Game(unsigned int screen_width, unsigned int screen_height,
         const std::string& title, unsigned int target_fps) noexcept;
    ~Game() {}

    void Run();
    void HandleKeyboardEvents();

private:
    static inline fs::path resource_path_ =
        fs::current_path().append("resources");
    unsigned int level_ = 1;
    unsigned int target_fps_ = 60;
    unsigned int screen_width_;
    unsigned int screen_height_;
    std::string  title_ = "Roguelike";
    rl::Window   window_;
};

rl::Texture LoadTexture(const fs::path& texture_path);

}  // namespace game
