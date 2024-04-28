#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numbers>

namespace util {

inline auto Perpendicular(const sf::Vector2f& lineVector) -> sf::Vector2f {
  return {-lineVector.y, lineVector.x};
}

inline auto Normalize(const sf::Vector2f& source) -> sf::Vector2f {
  float length = std::sqrt(source.x * source.x + source.y * source.y);
  if (length != 0) {
    return {source.x / length, source.y / length};
  }
  return source;
}

inline auto Line(const sf::Vector2f& point1, const sf::Vector2f& point2,
                 const float thickness, [[maybe_unused]] const sf::Color& color)
    -> sf::VertexArray {
  auto direction = point2 - point1;
  auto perp = Perpendicular(Normalize(direction));

  sf::VertexArray quad(sf::TriangleStrip, 4);
  quad[0].position = point1 + perp * thickness;
  quad[1].position = point1 - perp * thickness;
  quad[2].position = point2 + perp * thickness;
  quad[3].position = point2 - perp * thickness;

  quad[0].color = sf::Color::Red;
  quad[1].color = sf::Color::Red;
  quad[2].color = sf::Color::Red;
  quad[3].color = sf::Color::Red;

  return quad;
}

inline auto Near(const float lhs, const float rhs) -> bool {
  return std::abs(lhs - rhs) < 0.0001F;
}

}  // namespace util