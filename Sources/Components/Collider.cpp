#include "Components/Collider.hpp"
namespace Gg {

  namespace Component {
    Collider::Collider():
      bbmin{glm::vec3{-0.5f,-0.5f,-0.5f}},
      bbmax{glm::vec3{0.5f,0.5f,0.5f}}
    {}
    Collider::Collider(const glm::vec3 bmi,const glm::vec3 bma):
      bbmin{bmi},
      bbmax{bma}
    {}

    Collider::Collider(const Collider &col):
    	bbmin{col.bbmin},
      bbmax{col.bbmax}
     {}
  }
}
