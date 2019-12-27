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

        glm::vec3 acceleration = eForces->forces;
        acceleration /= eForces->mass;
        eForces->velocity = eForces->velocity + acceleration;
        if(glm::length(eForces->velocity )>1.f){
          eForces->velocity =1.f*glm::normalize(eForces->velocity );
        }
        std::static_pointer_cast<Gg::Component::Transformation>(m_gulgEngine.getComponent(currentEntity, "Transformations"))->translate(eForces->velocity);

        eForces->forces = glm::vec3(0.f,0.f,eForces->gravity_f);
      }
      //Acceleration = Forces / Mass
      //Velocity = Velocity + acceleration * time
      //Position = Position + velocity * time

    }
  }
}
