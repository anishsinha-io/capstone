#include "gameobject.h"

namespace game {

GameObject::GameObject(const rl::Vector2& position) : position_{position} {}
GameObject::GameObject(float x, float y) : position_{x, y} {}
GameObject::GameObject(const rl::Vector2& position, float scale)
    : position_{position}, scale_{scale} {}

}  // namespace game
