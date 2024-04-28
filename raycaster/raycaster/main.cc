#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numbers>

#include "raycaster/constants.h"
#include "raycaster/player.h"
#include "raycaster/util.h"

namespace raycaster {

auto HorizontalIntersect(const float angle, const float x, const float y)
    -> sf::Vector2f {
  const auto arc_tan =
      -1.0F / std::tan(angle * (std::numbers::pi_v<float> / 180));
  const auto block_size = static_cast<int>(SCREEN_DIMENSION / level.size());
  auto ry = std::floor(y / block_size) * block_size - 0.0001F;
  auto rx = (y - ry) * arc_tan + x;
  auto yo = block_size * -1;
  auto xo = -1 * yo * arc_tan;

  if (angle > 180.0F && angle < 360.0F) {
    ry = std::floor(y / block_size) * block_size + block_size;
    rx = (y - ry) * arc_tan + x;
    yo = block_size;
    xo = -xo;
  }

  if (util::Near(angle, 180.0F) || util::Near(angle, 0.0F)) {
    ry = y;
    rx = x;
  } else {
    for (size_t i = 0; i < 10; i++) {
      auto row = std::floor((ry / SCREEN_DIMENSION) * level.size());
      auto col = std::floor((rx / SCREEN_DIMENSION) * level.size());
      if (row < 0 || row >= level.size() || col < 0 || col >= level.size() ||
          level[row][col] == 1) {
        break;
      }
      rx += xo;
      ry += yo;
    }
  }

  return sf::Vector2f{rx, ry};
}

auto VerticalIntersect(const float angle, const float x, const float y)
    -> sf::Vector2f {
  const auto tan = std::tan(angle * (std::numbers::pi_v<float> / 180));
  const auto block_size = static_cast<int>(SCREEN_DIMENSION / level.size());
  auto rx = std::floor(x / block_size) * block_size - 0.0001F;
  auto ry = -1 * (x - rx) * tan + y;
  auto xo = -block_size;
  auto yo = xo * tan;

  if (angle > 90.0F && angle < 270.0F) {
    rx = std::floor(x / block_size) * block_size + block_size;
    ry = -1 * (x - rx) * tan + y;
    xo = block_size;
    yo = -yo;
  }

  if (util::Near(angle, 90.0F) || util::Near(angle, 270.0F)) {
    rx = x;
    ry = y;
  } else {
    for (size_t i = 0; i < 10; i++) {
      auto row = std::floor((ry / SCREEN_DIMENSION) * level.size());
      auto col = std::floor((rx / SCREEN_DIMENSION) * level.size());
      if (row < 0 || row >= level.size() || col < 0 || col >= level.size() ||
          level[row][col] == 1) {
        break;
      }
      rx += xo;
      ry += yo;
    }
  }

  return sf::Vector2f{rx, ry};
}

auto CastRay(const float angle, const float x, const float y) -> sf::Vector2f {
  auto vert = HorizontalIntersect(angle, x, y);
  auto horz = VerticalIntersect(angle, x, y);
  auto distance1 =
      std::sqrt(std::pow(vert.x - x, 2.0F) + std::pow(vert.y - y, 2.0F));
  auto distance2 =
      std::sqrt(std::pow(horz.x - x, 2.0F) + std::pow(horz.y - y, 2.0F));

  if (util::Near(angle, 90.0F) || util::Near(angle, 270.0F)) {
    return vert;
  }
  if (util::Near(angle, 180.0F) || util::Near(angle, 0.0F)) {
    return horz;
  }

  if (distance1 < distance2) {
    return vert;
  }

  return horz;
}

void DrawRaysDebug(sf::RenderWindow &window, const Player &player) {
  auto location = player.GetLocation();
  auto angle = player.GetAngle();
  auto horizontal_intersect =
      HorizontalIntersect(angle, location.x, location.y);
  auto vertical_intersect = VerticalIntersect(angle, location.x, location.y);
  auto horizontal_intersect_line =
      util::Line(sf::Vector2f(location.x, location.y),
                 sf::Vector2f(horizontal_intersect.x, horizontal_intersect.y),
                 3, sf::Color::Red);
  window.draw(horizontal_intersect_line);
  sf::Vertex vertical_intersect_line[] = {
      sf::Vertex(sf::Vector2f(location.x, location.y), sf::Color::Blue),
      sf::Vertex(sf::Vector2f(vertical_intersect.x, vertical_intersect.y),
                 sf::Color::Blue)};
  window.draw(vertical_intersect_line, 2, sf::Lines);
}

void DrawWalls(sf::RenderWindow &window, const Player &player) {
  auto location = player.GetLocation();
  auto angle = player.GetAngle();
  for (float i = -30; i < 30; i += 1.0F) {
    auto ray = CastRay(angle + i, location.x, location.y);
    auto dist = std::sqrt(std::pow(ray.x - location.x, 2.0F) +
                          std::pow(ray.y - location.y, 2.0F));
    auto block_size = static_cast<int>(SCREEN_DIMENSION / level.size());
    auto line_height =
        std::min((block_size * MAX_WALL_HEIGHT) / dist, MAX_WALL_HEIGHT);
    auto line_offset = MAX_WALL_HEIGHT - line_height / 2;
    auto screen_dimension_offset = SCREEN_DIMENSION / 2;
    auto start = sf::Vector2f(
        i * 6 + SCREEN_DIMENSION + screen_dimension_offset, line_offset);
    auto end = sf::Vector2f(i * 6 + SCREEN_DIMENSION + screen_dimension_offset,
                            line_height + line_offset);
    auto quad = util::Line(start, end, 3, sf::Color::Red);
    window.draw(quad);
  }
}

void DrawRays(sf::RenderWindow &window, const Player &player) {
  auto location = player.GetLocation();
  auto angle = player.GetAngle();
  for (float i = -30; i < 30; i += 1.0F) {
    auto ray = CastRay(angle + i, location.x, location.y);
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(location.x, location.y), sf::Color::Red),
        sf::Vertex(sf::Vector2f(ray.x, ray.y), sf::Color::Red)};
    window.draw(line, 2, sf::Lines);
  }
}

void DrawPlayer(sf::RenderWindow &window, const Player &player) {
  auto radius = 10.0F;
  sf::CircleShape triangle{radius, 3};
  triangle.setFillColor(sf::Color::Green);
  auto position = player.GetLocation();
  triangle.setPosition(position.x, position.y);
  triangle.setOrigin(radius, radius);
  sf::Transform transform;
  transform.rotate(player.GetAngle() + 270, position.x, position.y);
  window.draw(triangle, transform);
}

void DrawLevel(sf::RenderWindow &window) {
  auto width = SCREEN_DIMENSION / level.size();
  sf::RectangleShape rect{sf::Vector2f(width, width)};
  rect.setFillColor(sf::Color::Black);
  rect.setOutlineColor(sf::Color::Black);
  rect.setOutlineThickness(1);
  for (size_t i = 0; i < level.size(); i++) {
    for (size_t j = 0; j < level[i].size(); j++) {
      rect.setPosition(i * width, j * width);
      if (level[j][i] == 1) {
        rect.setFillColor(sf::Color::Black);
      } else {
        rect.setFillColor(sf::Color::White);
      }
      window.draw(rect);
    }
  }
}

}  // namespace raycaster

auto main() -> int {
  raycaster::Player player;
  sf::Vector2f start{100, 100};
  player.SetLocation(start);

  auto window = sf::RenderWindow{
      {raycaster::SCREEN_DIMENSION * 2, raycaster::SCREEN_DIMENSION},
      "Raycaster",
      sf::Style::Titlebar};

  window.setFramerateLimit(144);
  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::EventType::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Left:
            player.Turn(-10);
            break;
          case sf::Keyboard::Right:
            player.Turn(10);
            break;
          case sf::Keyboard::Up:
            player.Move(-10);
            break;
          case sf::Keyboard::Down:
            player.Move(10);
            break;
          case sf::Keyboard::Q:
            window.close();
          default:
            break;
        }
      }

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();

    raycaster::DrawLevel(window);
    raycaster::DrawPlayer(window, player);
    raycaster::DrawRays(window, player);
    raycaster::DrawWalls(window, player);
    raycaster::DrawRaysDebug(window, player);

    window.display();
  }

  return 0;
}
