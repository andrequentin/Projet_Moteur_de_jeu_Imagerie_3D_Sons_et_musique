#ifndef EXPLOSIVE_HPP
#define EXPLOSIVE_HPP

#include <glm/vec3.hpp>

#include "Components/Component.hpp"
enum explosiveTrigger {
  TIMER,
  ON_COLLISION
};
namespace Gg {

  namespace Component {

    class Explosive: public Gg::Component::AbstractComponent{
    public:

      Explosive();

      Explosive(int eP,explosiveTrigger eT);

      Explosive(const Explosive &exp);

      virtual std::shared_ptr<AbstractComponent> clone() const{

        return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<Explosive>(*this));
      }
      int explosivePower;
      explosiveTrigger eTrigger;
      
    };
  }
}
#endif
