#include "Components/Collider.hpp"
namespace Gg {

  namespace Component {
    Collider::Collider():
      c1{glm::vec3{0.5f,0.5f,0.5f}},
      c2{glm::vec3{0.5f,0.5f,0.5f}},
      r{0.5f}
    {}
    Collider::Collider(const glm::vec3 c1,const glm::vec3 c2, const float r):
      c1{c1},
      c2{c2},
      r{r}
    {}

    Collider::Collider(const Collider &col):
    	c1{col.c1},
      c2{col.c2},
      r{col.r}
     {}
  }
}
