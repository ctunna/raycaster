#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numbers>

namespace raycaster {
class Player {
 public:
  void Move(float distance);
  void Turn(float degrees);
  void SetLocation(const sf::Vector2f &pos) { position = pos; }
  [[nodiscard]] auto GetLocation() const -> sf::Vector2f;
  [[nodiscard]] auto GetAngle() const -> float;
  void SetAngle(float degrees);

 private:
  sf::Vector2f position{0, 0};
  float angle{90.0};
};
}  // namespace raycaster
