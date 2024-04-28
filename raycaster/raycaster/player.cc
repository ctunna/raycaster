#include "raycaster/player.h"

namespace raycaster {
void Player::Move(float distance) {
  float radians{angle * (std::numbers::pi_v<float> / 180)};
  float opposite{std::sin(radians) * distance};
  float adjacent{std::cos(radians) * distance};
  position.x += adjacent;
  position.y += opposite;
}

void Player::Turn(float degrees) {
  angle += degrees;
  if (angle >= 360) {
    angle -= 360;
  } else if (angle < 0) {
    angle += 360;
  }
}

auto Player::GetLocation() const -> sf::Vector2f { return position; }

auto Player::GetAngle() const -> float { return angle; }

auto Player::SetAngle(float degrees) -> void { angle = degrees; }

};  // namespace raycaster
