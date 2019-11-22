#ifndef MATHS_BASICS_HPP
#define MATHS_BASICS_HPP

#include <cmath>

#include "Components/BasicComponents.hpp"

namespace Gg {

namespace Maths {

float distance(const Component::Vector2D &pos1, const Component::Vector2D &pos2);
Component::Vector2D vectorFromPoints(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
float determinant(const sf::Vector2f &v1, const sf::Vector2f &v2);
float scalarProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);

}}

#endif