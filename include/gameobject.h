#pragma once

#include <raylib-cpp.hpp>

namespace game {

namespace rl = raylib;

/**
 * @class GameObject
 * @brief A drawable and updatable game object. Base class for everything in the
 * game.
 *
 */
class GameObject {
public:
    GameObject(const rl::Vector2& position);
    GameObject(float x, float y);
    virtual ~GameObject() = default;

    virtual void Draw() = 0;
    virtual void Update() = 0;

    inline auto GetPosition() const -> const rl::Vector2& { return position_; }
    inline void SetPosition(const rl::Vector2& position) {
        position_ = position;
    }

    inline auto GetPositionX() const -> float { return position_.x; }
    inline auto GetPositionY() const -> float { return position_.y; }

    inline void SetPosition(float x, float y) { position_ = rl::Vector2{x, y}; }
    inline void SetPositionX(float x) { position_.x = x; }
    inline void SetPositionY(float y) { position_.y = y; }

protected:
    rl::Vector2 position_;
};

/**
 * @class Textured
 * @brief A textured game object.
 *
 */
class Textured : virtual public GameObject {
public:
    Textured(rl::Texture&& texture) : texture_{std::move(texture)} {}
    Textured(const rl::Vector2& position, rl::Texture&& texture)
        : GameObject(position), texture_{std::move(texture)} {}

protected:
    rl::Texture texture_;
};

/**
 * @class Moveable
 * @brief A moveable game object.
 *
 */
class Moveable : virtual public GameObject {
public:
    Moveable() = default;

    enum class Direction { kUp, kDown, kLeft, kRight };

    virtual void Move(Direction direction) = 0;
};

}  // namespace game
