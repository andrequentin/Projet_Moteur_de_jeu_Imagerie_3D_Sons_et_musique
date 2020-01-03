#ifndef TIMER_HPP
#define TIMER_HPP

#include <glm/vec3.hpp>
#include <chrono>
#include "Components/Component.hpp"


namespace Gg {

  namespace Component {

    class Timer: public Gg::Component::AbstractComponent{
    public:

      Timer();

      Timer(long millisecs);

      Timer(const Timer &t);

      virtual std::shared_ptr<AbstractComponent> clone() const{

        return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<Timer>(*this));
      }
      std::chrono::time_point<std::chrono::system_clock> end;

    };
  }
}
#endif
