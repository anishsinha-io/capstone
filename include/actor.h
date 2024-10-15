#pragma once

#include "platforms.h"
#include "player.h"

namespace game {

using Actor = std::variant<Player, UntexturedPlatform>;

}
