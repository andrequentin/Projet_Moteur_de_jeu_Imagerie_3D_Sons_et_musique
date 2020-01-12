#ifndef FORCES_HPP
#define FORCES_HPP

#include <glm/vec3.hpp>
#include <iostream>
#include "Components/Component.hpp"

namespace Gg {

  namespace Component {
    class Forces: public Gg::Component::AbstractComponent{

    public:

      Forces();

      Forces(const glm::vec3 f,float gf,float m,float ms  );

      Forces(const Forces &fs);

      void addForce(glm::vec3 f);

      virtual std::shared_ptr<AbstractComponent> clone() const{

        return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<Forces>(*this));
      }

      //Acceleration = Forces / Mass
      //Velocity = Velocity + acceleration * time
      //Position = Position + velocity * time

      glm::vec3 velocity;
      glm::vec3 forces;
      float mass;
      float gravity_f;
      float maxspeed;

    };
  }
}
#endif
