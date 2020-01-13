#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <glm/vec3.hpp>

#include "Components/Component.hpp"

namespace Gg {

  namespace Component {
    class Collider: public Gg::Component::AbstractComponent{

    public:

      Collider();

      Collider(const glm::vec3 bmi,const glm::vec3 bma);

      Collider(const Collider &col);

      virtual std::shared_ptr<AbstractComponent> clone() const{

        return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<Collider>(*this));
      }


      glm::vec3 bbmin,bbmax;
    };
  }
}
#endif
