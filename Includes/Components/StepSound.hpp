#ifndef STEPSOUND_HPP
#define STEPSOUND_HPP

#include <glm/vec3.hpp>
#include <iostream>
#include "Components/Component.hpp"
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>

namespace Gg {

  namespace Component {
    class StepSound: public Gg::Component::AbstractComponent{

    public:

      StepSound();

      StepSound(FMOD::Studio::EventInstance*s);

      virtual std::shared_ptr<AbstractComponent> clone() const{

        return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<StepSound>(*this));
      }



      FMOD::Studio::EventInstance* stepeventInstance;
    };
  }
}
#endif
