#include "Mathematics/Basics.hpp"

namespace Gg {

namespace Maths {

float distance(const Component::Vector2D &pos1, const Component::Vector2D &pos2) {

	return sqrt((pos2.value.x - pos1.value.x)*(pos2.value.x - pos1.value.x) 
			  + (pos2.value.y - pos1.value.y)*(pos2.value.y - pos1.value.y));
}

Component::Vector2D vectorFromPoints(const sf::Vector2f &pos1, const sf::Vector2f &pos2) {

	return Component::Vector2D{pos2.x - pos1.x, pos2.y - pos1.y};
}

float determinant(const sf::Vector2f &v1, const sf::Vector2f &v2) {

	return v1.x*v2.y - v1.y*v2.x;
}

float scalarProduct(const sf::Vector2f &v1, const sf::Vector2f &v2) {

	return v1.x*v2.x + v1.y*v2.y;
}

}}