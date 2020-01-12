#include "Components/Forces.hpp"
namespace Gg {

  namespace Component {
    Forces::Forces():
    velocity{glm::vec3{0.0f}},
    forces{glm::vec3{0.0f}},
    mass{1.f},
    gravity_f{0.1f},
    maxspeed{2.f}
    {}
    Forces::Forces(const glm::vec3 f,float gf,float m,float ms) :
      velocity{glm::vec3{0.0f}},
      forces{f},
      mass{m},
      gravity_f{gf},
      maxspeed{ms}
    {}

    Forces::Forces(const Forces &fs):
    velocity{fs.velocity},
    forces{fs.forces},
    mass{fs.mass},
    gravity_f{fs.gravity_f},
    maxspeed{fs.maxspeed}
     {}

    void Forces::addForce(glm::vec3 f){
      forces +=f;
    }

  }
}
