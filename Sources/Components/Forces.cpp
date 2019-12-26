#include "Components/Forces.hpp"
namespace Gg {

  namespace Component {
    Forces::Forces():
    velocity{glm::vec3{0.0f}},
    forces{glm::vec3{0.0f}},
    mass{1.f},
    gravity_f{0.02f}
    {}
    Forces::Forces(const glm::vec3 f,float gf,float m) :
      velocity{glm::vec3{0.0f}},
      forces{f},
      mass{m},
      gravity_f{gf}
    {}

    Forces::Forces(const Forces &fs):
    velocity{fs.velocity},
    forces{fs.forces},
    mass{fs.mass},
    gravity_f{fs.gravity_f}
     {}

    void Forces::addForce(glm::vec3 f){
      forces +=f;
    }

  }
}
