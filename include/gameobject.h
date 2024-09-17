#pragma once

#include <raylib-cpp.hpp>

namespace game {

namespace rl = raylib;

class GameObject {
public:
    GameObject(const rl::Vector2& position);
    GameObject(float x, float y);
    virtual ~GameObject() = default;

    virtual void Draw() = 0;

protected:
    rl::Vector2 position_;
};

class Textured : virtual public GameObject {
public:
    Textured(rl::Texture&& texture) : texture_{std::move(texture)} {}
    Textured(const rl::Vector2& position, rl::Texture&& texture)
        : GameObject(position), texture_{std::move(texture)} {}

protected:
    rl::Texture texture_;
};

class Moveable : virtual public GameObject {
public:
    Moveable() = default;

    enum class Direction { kUp, kDown, kLeft, kRight };

    virtual void Move(Direction direction) = 0;
};

}  // namespace game
