#include <raylib-cpp.hpp>

#include "game.h"

int main(int argc, char** argv) {
    using namespace game;

    Game game(1200, 675, "roguelike", 60);
    game.Run();

    return 0;
}
