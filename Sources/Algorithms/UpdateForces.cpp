#include "Algorithms/UpdateForces.hpp"


namespace Gg {

  namespace Algorithm {

    UpdateForces::UpdateForces(Gg::GulgEngine &gulgEngine):
    	AbstractAlgorithm{gulgEngine} {

      m_signature = gulgEngine.getComponentSignature("Forces");

    }

    UpdateForces::~UpdateForces() {}

    void UpdateForces::apply() {
      for(Gg::Entity currentEntity: m_entitiesToApply) {
        std::shared_ptr<Gg::Component::Forces> eForces {
          std::static_pointer_cast<Gg::Component::Forces>(m_gulgEngine.getComponent(currentEntity, "Forces"))
        };

        glm::vec3 acceleration = eForces->forces  ;
        acceleration /= eForces->mass;
        // acceleration *= -1.f;
        eForces->velocity = eForces->velocity + acceleration;
        std::static_pointer_cast<Gg::Component::Transformation>(m_gulgEngine.getComponent(currentEntity, "Transformations"))->translate(eForces->velocity);

        eForces->velocity -= eForces->velocity/3.f;
        eForces->forces = glm::vec3(0.0f,0.0f,1.f*eForces->gravity_f);
      }
      //Acceleration = Forces / Mass
      //Velocity = Velocity + acceleration * time
      //Position = Position + velocity * time

    }
  }
}
