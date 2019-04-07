#pragma once

#include <SFML/Graphics/Vertex.hpp>

struct Vertex : public sf::Vertex {

  inline bool operator==(sf::Vertex rhs) const {
    return position == rhs.position;
  }

  inline bool operator!=(sf::Vertex rhs) const {
    return position != rhs.position;
  }
};
